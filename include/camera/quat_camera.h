#pragma once
#include <memory>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "camera/camera.h"

namespace graphics::camera {
class QuaternionCamera final : public Camera {
 public:
  explicit QuaternionCamera(const glm::vec3& _position) : Camera(_position), rotation(glm::identity<glm::quat>()) {}
  bool move(GLFWwindow* window) override;
  void updateView() override;
  void updateProjection(float aspectRatio) override;
  static std::unique_ptr<QuaternionCamera> make_unique(const glm::vec3& _position);

 private:
  glm::quat rotation;
  // TODO (optional): Change these values if your WASD or mouse move too slow or too fast.
  constexpr static float keyboardMoveSpeed = 0.1f;
  constexpr static float mouseMoveSpeed = 0.001f;
};
using QuaternionCameraPTR = std::unique_ptr<QuaternionCamera>;
}  // namespace graphics::camera
