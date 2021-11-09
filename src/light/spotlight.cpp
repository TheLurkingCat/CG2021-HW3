#include "light/spotlight.h"

#include <glm/gtc/matrix_transform.hpp>

namespace graphics::light {
Spotlight::Spotlight(const glm::vec4& lightdir, const glm::vec2& lightcoef) noexcept :
    Light(lightdir, glm::vec4(lightcoef, 1, 0)) {
  // Spotlight's projection matrix.
  projectionMatrix = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -75.0f, 75.0f);
}

void Spotlight::update(const glm::mat4& viewMatrix) {
  // Spotlight's projection * view matrix.
  setLightSpaceMatrix(projectionMatrix * viewMatrix);
}

SpotlightPTR Spotlight::make_unique(const glm::vec3& lightdir, const glm::vec2& lightcoef) {
  return std::make_unique<Spotlight>(glm::vec4(lightdir, 0.0), lightcoef);
}

SpotlightPTR Spotlight::make_unique(const glm::vec4& lightdir, const glm::vec2& lightcoef) {
  return std::make_unique<Spotlight>(lightdir, lightcoef);
}
}  // namespace graphics::light
