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
int currentShader = 1;
int alignSize = 256;

float fresnelBias = 0.5f;
float fresnelScale = 0.4f;
float fresnelPower = 1;
bool updateFresnelParameters = true;

constexpr int CAMERA_COUNT = 1;
constexpr int MESH_COUNT = 1;
constexpr int SHADER_PROGRAM_COUNT = 2;
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
    // Disable mouse cursor.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else if (key == GLFW_KEY_F8) {
    // Enable mouse cursor.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void renderMainPanel();
void renderGUI();

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
  std::string filenames[SHADER_PROGRAM_COUNT] = {"skybox", "part1"};
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

    shaderPrograms[i].setUniform("diffuseTexture", 0);
    shaderPrograms[i].setUniform("skybox", 1);
  }
  graphics::buffer::UniformBuffer meshUBO, cameraUBO;
  // Calculate UBO alignment size
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignSize);
  constexpr int perMeshSize = 2 * sizeof(glm::mat4);
  constexpr int perCameraSize = sizeof(glm::mat4) + sizeof(glm::vec4);
  constexpr int perLightSize = sizeof(glm::mat4) + 2 * sizeof(glm::vec4);
  int perMeshOffset = uboAlign(perMeshSize);
  int perCameraOffset = uboAlign(perCameraSize);
  int perLightOffset = uboAlign(perLightSize);
  meshUBO.allocate(MESH_COUNT * perMeshOffset, GL_DYNAMIC_DRAW);
  cameraUBO.allocate(CAMERA_COUNT * perCameraOffset, GL_DYNAMIC_DRAW);
  // Default to first data
  meshUBO.bindUniformBlockIndex(0, 0, perMeshSize);
  cameraUBO.bindUniformBlockIndex(1, 0, perCameraSize);
  // Camera
  std::vector<graphics::camera::CameraPTR> cameras;
  cameras.emplace_back(graphics::camera::QuaternionCamera::make_unique(glm::vec3(0, 0, 15)));
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
  skybox.fromFile("../assets/texture/posx.jpg", "../assets/texture/negx.jpg", "../assets/texture/posy.jpg",
                  "../assets/texture/negy.jpg", "../assets/texture/posz.jpg", "../assets/texture/negz.jpg", false);

  // Meshes
  std::vector<graphics::shape::ShapePTR> meshes;
  {
    auto sphere = graphics::shape::Sphere::make_unique();

    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(3, 0, 4));
    model = glm::scale(model, glm::vec3(2));
    model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    sphere->setModelMatrix(model);

    meshes.emplace_back(std::move(sphere));
  }
  // The cube for skybox
  auto skyboxCube = graphics::shape::Cube::make_unique();

  assert(meshes.size() == MESH_COUNT);
  for (int i = 0; i < MESH_COUNT; ++i) {
    int offset = i * perMeshOffset;
    meshUBO.load(offset, sizeof(glm::mat4), meshes[i]->getModelMatrixPTR());
    meshUBO.load(offset + sizeof(glm::mat4), sizeof(glm::mat4), meshes[i]->getNormalMatrixPTR());
  }
  skybox.bind(1);
  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera's uniforms if camera moves.
    bool isCameraMove = currentCamera->move(window);
    if (isCameraMove || isWindowSizeChanged) {
      isWindowSizeChanged = false;
      shaderPrograms[0].use();
      shaderPrograms[0].setUniformMatrix("view", currentCamera->getViewMatrixPTR());
      shaderPrograms[0].setUniformMatrix("projection", currentCamera->getProjectionMatrixPTR());

      cameraUBO.load(0, sizeof(glm::mat4), currentCamera->getViewProjectionMatrixPTR());
      cameraUBO.load(sizeof(glm::mat4), sizeof(glm::vec4), currentCamera->getPositionPTR());
    }
    shaderPrograms[currentShader].use();
    // Update fresnel equation's parametsers.
    if (updateFresnelParameters) {
      shaderPrograms[currentShader].setUniform("fresnelBias", fresnelBias);
      shaderPrograms[currentShader].setUniform("fresnelScale", fresnelScale);
      shaderPrograms[currentShader].setUniform("fresnelPower", fresnelPower);
    }
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render all objects
    for (int i = 0; i < MESH_COUNT; ++i) {
      // Bind current object's model matrix
      meshUBO.bindUniformBlockIndex(0, i * perMeshOffset, perMeshSize);
      // Render current object
      meshes[i]->draw();
    }
    // Render skybox after meshes
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);
    shaderPrograms[0].use();
    skyboxCube->draw();
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
    // Render GUI
    renderGUI();
#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}
void renderMainPanel() {
  ImGui::SetNextWindowSize(ImVec2(320.0f, 100.0f), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(0, ImGuiCond_Once);
  ImGui::SetNextWindowPos(ImVec2(50.0f, 50.0f), ImGuiCond_Once);
  ImGui::SetNextWindowBgAlpha(0.2f);
  bool dirty = false;
  if (ImGui::Begin("Configs")) {
    dirty |= ImGui::SliderFloat("Fresnel bias", &fresnelBias, 0, 1, "%.2f");
    dirty |= ImGui::SliderFloat("Fresnel scale", &fresnelScale, 0, 1, "%.2f");
    dirty |= ImGui::SliderFloat("Fresnel power", &fresnelPower, 0, 100, "%.1f");
    ImGui::End();
  }
  updateFresnelParameters = dirty;
}

void renderGUI() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  renderMainPanel();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
