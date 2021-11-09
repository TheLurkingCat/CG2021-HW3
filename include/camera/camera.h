#pragma once
#include <memory>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace graphics::camera {

class Camera {
 public:
  explicit Camera(const glm::vec3& position);
  virtual ~Camera() = default;

  glm::vec4 getPosition() const { return glm::vec4(position, 1.0); }
  const float* getPositionPTR() const { return glm::value_ptr(position); }
  glm::vec4 getFront() const { return glm::vec4(front, 0.0); }
  const float* getFrontPTR() const { return glm::value_ptr(front); }
  glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
  const float* getProjectionMatrixPTR() const { return glm::value_ptr(projectionMatrix); }
  glm::mat4 getViewMatrix() const { return viewMatrix; }
  const float* getViewMatrixPTR() const { return glm::value_ptr(viewMatrix); }
  glm::mat4 getViewProjectionMatrix() const { return projectionMatrix * viewMatrix; }
  const float* getViewProjectionMatrixPTR() const { return glm::value_ptr(viewProjectionMatrix); }
  void initialize(float aspectRatio);
  virtual bool move(GLFWwindow* window) = 0;
  virtual void updateProjection(float aspectRatio) = 0;

 protected:
  virtual void updateView() = 0;
  glm::vec3 position;
  glm::vec3 up;
  glm::vec3 front;
  glm::vec3 right;
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 viewProjectionMatrix;
};
using CameraPTR = std::unique_ptr<Camera>;
}  // namespace graphics::camera
