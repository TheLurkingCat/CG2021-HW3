#include "light/directionallight.h"
#include <glm/gtc/matrix_transform.hpp>

namespace graphics::light {
DirectionalLight::DirectionalLight(const glm::vec3& lightDirection) noexcept :
    Light(glm::vec4(lightDirection, 0), glm::vec4(0, 0, 0, 1)) {
  glm::mat4 viewProjection(1);
  // TODO (If you want to implement shadow): Set view projection matrix for light
  // Hint:
  //       1. You should use orthographic projection for directional light.
  //         zNear, zFar, bottom, top, left, right = -75.0, 75.0, -30.0, 30.0, -30.0, 30.0
  //         glm::ortho (https://glm.g-truc.net/0.9.9/api/a00665.html#ga6615d8a9d39432e279c4575313ecb456)
  //       2. You need to calculate the view matrix too
  viewProjection *= glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -75.0f, 75.0f);
  viewProjection *= glm::lookAt(lightDirection, glm::vec3(0), glm::vec3(0, 1, 0));
  setLightSpaceMatrix(viewProjection);
}
DirectionalLightPTR DirectionalLight::make_unique(const glm::vec3& lightDirection) {
  return std::make_unique<DirectionalLight>(lightDirection);
}
}  // namespace graphics::light
