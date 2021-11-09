#include "camera/camera.h"
namespace graphics::camera {

Camera::Camera(const glm::vec3& _position)
    : position(_position), up(0, 1, 0), front(0, 0, -1), right(1, 0, 0), projectionMatrix(1), viewMatrix(1) {}

void Camera::initialize(float aspectRatio) {
  updateProjection(aspectRatio);
  updateView();
}
}  // namespace graphics::camera
