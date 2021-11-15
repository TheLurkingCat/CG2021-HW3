#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "graphics.h"

// Unnamed namespace for global variables
namespace {
// Cameras
graphics::camera::Camera* currentCamera = nullptr;
// Control variables
bool isWindowSizeChanged = true;
int alignSize = 256;

float fresnelBias = 0;
float fresnelScale = 1;
float fresnelPower = 1;
bool updateFresnelParameters = true;
float rotation = 0.0f;
bool updateRotation = true;
bool useDisplacement = false;
bool useParallax = false;
bool updateMapping = false;
bool mouseBinded = false;

// TODO (Bonus-Displacement): Change 'planeSubDivision' to >= 100, otherwise displacement mapping will not look good.
constexpr int planeSubDivision = 1;
constexpr int CAMERA_COUNT = 1;
constexpr int MESH_COUNT = 3;
constexpr int SHADER_PROGRAM_COUNT = 4;
constexpr int normalMapSize = 1024;
}  // namespace

int uboAlign(int i) { return ((i + 1 * (alignSize - 1)) / alignSize) * alignSize; }

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold
  if (action != GLFW_PRESS) return;
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  } else if (key == GLFW_KEY_F9) {
    // Disable / enable mouse cursor.
    if (mouseBinded)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouseBinded = !mouseBinded;
  }
}

void renderMainPanel(graphics::texture::Texture* normalmap, graphics::texture::Texture* heightmap);
void renderGUI(graphics::texture::Texture* normalmap, graphics::texture::Texture* heightmap);

void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  assert(currentCamera != nullptr);
  currentCamera->updateProjection(OpenGLContext::getAspectRatio());
  isWindowSizeChanged = true;
}

int main() {
  // Initialize OpenGL context, details are wrapped in class.
  OpenGLContext::createContext(43, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = OpenGLContext::getWindow();
  glfwSetWindowTitle(window, "HW3");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
  // Initialize ImGUI
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  if (OpenGLContext::getOpenGLVersion() >= 41) {
    ImGui_ImplOpenGL3_Init("#version 410 core");
  } else {
    ImGui_ImplOpenGL3_Init(nullptr);
  }
  // Initialize shader
  std::vector<graphics::shader::ShaderProgram> shaderPrograms(SHADER_PROGRAM_COUNT);
  std::string filenames[SHADER_PROGRAM_COUNT] = {"skybox", "fresnel", "normalmap", "calculatenormal"};
  for (int i = 0; i < SHADER_PROGRAM_COUNT; ++i) {
    graphics::shader::VertexShader vs;
    graphics::shader::FragmentShader fs;
    vs.fromFile("../assets/shader/" + filenames[i] + ".vert");
    fs.fromFile("../assets/shader/" + filenames[i] + ".frag");
    shaderPrograms[i].attach(&vs, &fs);
    shaderPrograms[i].link();
    shaderPrograms[i].detach(&vs, &fs);
    shaderPrograms[i].use();

    shaderPrograms[i].uniformBlockBinding("model", 0);
    shaderPrograms[i].uniformBlockBinding("camera", 1);

    shaderPrograms[i].setUniform("useDisplacementMapping", 0);
    shaderPrograms[i].setUniform("useParallaxMapping", 0);
    shaderPrograms[i].setUniform("skybox", 0);
    shaderPrograms[i].setUniform("diffuseTexture", 1);
    shaderPrograms[i].setUniform("normalTexture", 2);
    shaderPrograms[i].setUniform("heightTexture", 3);
  }
  graphics::buffer::UniformBuffer meshUBO, cameraUBO;
  // Calculate UBO alignment size
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignSize);
  constexpr int perMeshSize = 2 * sizeof(glm::mat4);
  constexpr int perCameraSize = sizeof(glm::mat4) + sizeof(glm::vec4);
  int perMeshOffset = uboAlign(perMeshSize);
  int perCameraOffset = uboAlign(perCameraSize);
  meshUBO.allocate(MESH_COUNT * perMeshOffset, GL_DYNAMIC_DRAW);
  cameraUBO.allocate(CAMERA_COUNT * perCameraOffset, GL_DYNAMIC_DRAW);
  // Default to first data
  meshUBO.bindUniformBlockIndex(0, 0, perMeshSize);
  cameraUBO.bindUniformBlockIndex(1, 0, perCameraSize);
  // Camera
  std::vector<graphics::camera::CameraPTR> cameras;
  cameras.emplace_back(graphics::camera::QuaternionCamera::make_unique(glm::vec3(0, 2, 5)));
  assert(cameras.size() == CAMERA_COUNT);
  for (int i = 0; i < CAMERA_COUNT; ++i) {
    int offset = i * perCameraOffset;
    cameras[i]->initialize(OpenGLContext::getAspectRatio());
    cameraUBO.load(offset, sizeof(glm::mat4), cameras[i]->getViewProjectionMatrixPTR());
    cameraUBO.load(offset + sizeof(glm::mat4), sizeof(glm::vec4), cameras[i]->getPositionPTR());
  }
  currentCamera = cameras[0].get();

  // Texture
  graphics::texture::TextureCubeMap skybox;
  graphics::texture::Texture2D wood;
  graphics::texture::Framebuffer fbo;
  fbo.setBuffers({GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1}, GL_NONE);
  graphics::texture::ColorMap normalMap(normalMapSize, GL_RGBA32F);
  normalMap.attachtoFramebuffer(&fbo, GL_COLOR_ATTACHMENT0);
  graphics::texture::ColorMap heightMap(normalMapSize, GL_R32F);
  heightMap.attachtoFramebuffer(&fbo, GL_COLOR_ATTACHMENT1);
  skybox.fromFile("../assets/texture/posx.jpg", "../assets/texture/negx.jpg", "../assets/texture/posy.jpg",
                  "../assets/texture/negy.jpg", "../assets/texture/posz.jpg", "../assets/texture/negz.jpg", false);
  wood.fromFile("../assets/texture/wood.jpg");
  // Meshes
  std::vector<utils::Mesh> meshes;
  graphics::shape::Sphere sphere;
  graphics::shape::Cube skyboxCube;
  std::vector<GLfloat> vertex;
  std::vector<GLuint> index;
  graphics::shape::Plane::generateVertices(vertex, index, planeSubDivision);
  graphics::shape::Plane fakeWave(vertex, index);
  {
    using textureVector = std::vector<graphics::texture::Texture*>;
    meshes.emplace_back(&sphere, &shaderPrograms[1], textureVector{});
    meshes.emplace_back(&fakeWave, &shaderPrograms[2], textureVector{&skybox, &wood, &normalMap, &heightMap});
    meshes.emplace_back(&skyboxCube, &shaderPrograms[0], textureVector{&skybox});

    sphere.setModelMatrix(glm::translate(glm::mat4(1), glm::vec3(3, 0, 0)));
    skyboxCube.registerPreDrawFunction([] {
      glDepthFunc(GL_LEQUAL);
      glFrontFace(GL_CW);
    });
    skyboxCube.registerPostDrawFunction([] {
      glFrontFace(GL_CCW);
      glDepthFunc(GL_LESS);
    });
  }

  assert(meshes.size() == MESH_COUNT);
  for (int i = 0; i < MESH_COUNT; ++i) {
    int offset = i * perMeshOffset;
    meshUBO.load(offset, sizeof(glm::mat4), meshes[i].shape->getModelMatrixPTR());
    meshUBO.load(offset + sizeof(glm::mat4), sizeof(glm::mat4), meshes[i].shape->getNormalMatrixPTR());
  }
  int currentOffset = 0;
  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera's uniforms if camera moves.
    bool isCameraMove = mouseBinded ? currentCamera->move(window) : false;
    if (isCameraMove || isWindowSizeChanged) {
      isWindowSizeChanged = false;
      shaderPrograms[0].use();
      shaderPrograms[0].setUniformMatrix("view", currentCamera->getViewMatrixPTR());
      shaderPrograms[0].setUniformMatrix("projection", currentCamera->getProjectionMatrixPTR());

      cameraUBO.load(0, sizeof(glm::mat4), currentCamera->getViewProjectionMatrixPTR());
      cameraUBO.load(sizeof(glm::mat4), sizeof(glm::vec4), currentCamera->getPositionPTR());
    }
    shaderPrograms[1].use();
    // Update fresnel equation's parametsers.
    if (updateFresnelParameters) {
      shaderPrograms[1].setUniform("fresnelBias", fresnelBias);
      shaderPrograms[1].setUniform("fresnelScale", fresnelScale);
      shaderPrograms[1].setUniform("fresnelPower", fresnelPower);
      updateFresnelParameters = false;
    }
    if (updateRotation) {
      fakeWave.setModelMatrix(glm::rotate(glm::mat4(1), glm::radians(rotation), glm::vec3(1, 0, 0)));
      meshUBO.load(perMeshOffset, sizeof(glm::mat4), meshes[1].shape->getModelMatrixPTR());
      meshUBO.load(perMeshOffset + sizeof(glm::mat4), sizeof(glm::mat4), meshes[1].shape->getNormalMatrixPTR());
      updateRotation = false;
    }
    // update switches
    if (updateMapping) {
      shaderPrograms[2].use();
      shaderPrograms[2].setUniform("useParallaxMapping", useParallax);
      shaderPrograms[2].setUniform("useDisplacementMapping", useDisplacement);
      updateMapping = false;
    }
    // Update normal map
    fbo.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    (++currentOffset) %= 101;
    glViewport(0, 0, normalMapSize, normalMapSize);
    shaderPrograms[3].use();
    shaderPrograms[3].setUniform("offset", currentOffset * 0.01f * glm::two_pi<float>());
    meshUBO.bindUniformBlockIndex(0, perMeshOffset, perMeshSize);
    fakeWave.draw();
    glViewport(0, 0, OpenGLContext::getWidth(), OpenGLContext::getHeight());

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render all objects
    for (int i = 0; i < MESH_COUNT; ++i) {
      // Bind current object's model matrix
      meshUBO.bindUniformBlockIndex(0, i * perMeshOffset, perMeshSize);
      // Render current object
      meshes[i].draw();
    }
    // Render GUI
    renderGUI(&normalMap, &heightMap);
#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  return 0;
}

void renderMainPanel(graphics::texture::Texture* normalmap, graphics::texture::Texture* heightmap) {
  static bool normalMapButton = false;
  static bool heightMapButton = false;
  ImGui::SetNextWindowSize(ImVec2(400.0f, 275.0f), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(0, ImGuiCond_Once);
  ImGui::SetNextWindowPos(ImVec2(50.0f, 50.0f), ImGuiCond_Once);
  ImGui::SetNextWindowBgAlpha(0.2f);
  if (ImGui::Begin("Configs")) {
    ImGui::Text("----------------------- Part1 -----------------------");
    updateFresnelParameters |= ImGui::SliderFloat("Fresnel bias", &fresnelBias, 0, 1, "%.2f");
    updateFresnelParameters |= ImGui::SliderFloat("Fresnel scale", &fresnelScale, 0, 1, "%.2f");
    updateFresnelParameters |= ImGui::SliderFloat("Fresnel power", &fresnelPower, 0, 10, "%.1f");
    ImGui::Text("----------------------- Part2 -----------------------");
    updateRotation = ImGui::SliderFloat("Plane rotation", &rotation, 0, 90, "%.1f");
    if (ImGui::Button("Show normal map")) {
      normalMapButton = !normalMapButton;
    }
    ImGui::SameLine();
    if (ImGui::Button("Show height map")) {
      heightMapButton = !heightMapButton;
    }
    if (normalMapButton) {
      ImGui::SetNextWindowSize(ImVec2(271.0f, 291.0f), ImGuiCond_Once);
      ImGui::SetNextWindowCollapsed(0, ImGuiCond_Once);
      ImGui::SetNextWindowPos(ImVec2(460.0f, 50.0f), ImGuiCond_Once);
      ImGui::SetNextWindowBgAlpha(1.0f);
      if (ImGui::Begin("Normal Map")) {
        auto tex = reinterpret_cast<ImTextureID>(static_cast<std::uintptr_t>(normalmap->getHandle()));
        auto wsize = ImGui::GetWindowSize();
        wsize.x -= 15;
        wsize.y -= 35;
        ImGui::Image(tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
      }
      ImGui::End();
    }
    if (heightMapButton) {
      ImGui::SetNextWindowSize(ImVec2(271.0f, 291.0f), ImGuiCond_Once);
      ImGui::SetNextWindowCollapsed(0, ImGuiCond_Once);
      ImGui::SetNextWindowPos(ImVec2(740.0f, 50.0f), ImGuiCond_Once);
      ImGui::SetNextWindowBgAlpha(1.0f);
      if (ImGui::Begin("Height Map")) {
        auto tex = reinterpret_cast<ImTextureID>(static_cast<std::uintptr_t>(heightmap->getHandle()));
        auto wsize = ImGui::GetWindowSize();
        wsize.x -= 15;
        wsize.y -= 35;
        ImGui::Image(tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
      }
      ImGui::End();
    }
    ImGui::Text("----------------------- Bonus -----------------------");
    updateMapping |= ImGui::Checkbox("Displacement", &useDisplacement);
    ImGui::SameLine();
    updateMapping |= ImGui::Checkbox("Parallax", &useParallax);
    ImGui::Text("----------------------- Other -----------------------");
    ImGui::Text("Current framerate: %.0f", ImGui::GetIO().Framerate);
  }
  ImGui::End();
}

void renderGUI(graphics::texture::Texture* normalmap, graphics::texture::Texture* heightmap) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  renderMainPanel(normalmap, heightmap);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
