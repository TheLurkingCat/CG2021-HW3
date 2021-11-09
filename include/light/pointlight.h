#pragma once
#include <memory>
#include "light.h"
namespace graphics::light {
class PointLight final : public Light {
 public:
  PointLight(const glm::vec3& lightPosition) noexcept;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Point light"; }
  CONSTEXPR_VIRTUAL LightType getType() const override { return LightType::Point; }
  static std::unique_ptr<PointLight> make_unique(const glm::vec3& lightPosition);
};
using PointLightPTR = std::unique_ptr<PointLight>;
}  // namespace graphics::light
