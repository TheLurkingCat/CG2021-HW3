#include "camera/quat_camera.h"

namespace graphics::camera {
bool QuaternionCamera::move(GLFWwindow* window) {
  bool ismoved = false;
  // Mouse part
  static double lastx = 0, lasty = 0;
  if (lastx == 0 && lasty == 0) {
    glfwGetCursorPos(window, &lastx, &lasty);
  } else {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float dx = mouseMoveSpeed * static_cast<float>(xpos - lastx);
    float dy = mouseMoveSpeed * static_cast<float>(lasty - ypos);
    lastx = xpos;
    lasty = ypos;
    if (dx != 0 || dy != 0) {
      ismoved = true;
      glm::quat rx(glm::angleAxis(dx, glm::vec3(0, -1, 0)));
      glm::quat ry(glm::angleAxis(dy, glm::vec3(1, 0, 0)));
      rotation = rx * rotation * ry;
    }
  }
  // Keyboard part
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += front * keyboardMoveSpeed;
    ismoved = true;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= front * keyboardMoveSpeed;
    ismoved = true;
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= right * keyboardMoveSpeed;
    ismoved = true;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += right * keyboardMoveSpeed;
    ismoved = true;
  }
  // Update view matrix if moved
  if (ismoved) {
    updateView();
  }
  return ismoved;
}

void QuaternionCamera::updateView() {
  // TODO: paste your HW1 here
  constexpr glm::vec3 original_front(0, 0, -1);
  constexpr glm::vec3 original_up(0, 1, 0);
  front = rotation * original_front;
  up = rotation * original_up;
  right = glm::cross(front, up);
  viewMatrix = glm::lookAt(position, position + front, up);
  viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void QuaternionCamera::updateProjection(float aspectRatio) {
  // TODO: paste your HW1 here
  constexpr float FOV = glm::radians(45.0f);
  constexpr float zNear = 0.1f;
  constexpr float zFar = 100.0f;
  projectionMatrix = glm::perspective(FOV, aspectRatio, zNear, zFar);
  viewProjectionMatrix = projectionMatrix * viewMatrix;
}

std::unique_ptr<QuaternionCamera> QuaternionCamera::make_unique(const glm::vec3& _position) {
  return std::make_unique<QuaternionCamera>(_position);
}
}  // namespace graphics::camera
