#pragma once
#include <memory>
#include "light.h"
namespace graphics::light {
class Spotlight final : public Light {
 public:
  explicit Spotlight(const glm::vec4& lightdir, const glm::vec2& lightcoef) noexcept;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Spotlight"; }
  CONSTEXPR_VIRTUAL LightType getType() const override { return LightType::Spot; }
  static std::unique_ptr<Spotlight> make_unique(const glm::vec3& lightdir, const glm::vec2& lightcoef);
  static std::unique_ptr<Spotlight> make_unique(const glm::vec4& lightdir, const glm::vec2& lightcoef);
  void update(const glm::mat4& viewMatrix) override;

 private:
  glm::mat4 projectionMatrix;
};
using SpotlightPTR = std::unique_ptr<Spotlight>;
}  // namespace graphics::light
