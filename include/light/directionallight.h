#pragma once
#include <memory>
#include "light.h"
namespace graphics::light {
class DirectionalLight : public Light {
 public:
  explicit DirectionalLight(const glm::vec3& lightDirection) noexcept;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Directional light"; }
  CONSTEXPR_VIRTUAL LightType getType() const override { return LightType::Directional; }
  static std::unique_ptr<DirectionalLight> make_unique(const glm::vec3& lightDirection);
};
using DirectionalLightPTR = std::unique_ptr<DirectionalLight>;
}  // namespace graphics::light
