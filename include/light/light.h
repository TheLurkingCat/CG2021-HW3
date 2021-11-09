#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"

namespace graphics::light {
enum class LightType : uint8_t { Point, Directional, Spot };
class Light {
 public:
  Light(const glm::vec4& lightvector, const glm::vec4& lightcoef) noexcept :
      lightCoefficients(lightcoef), lightVector(lightvector) {}
  virtual ~Light() = default;

  void setLightSpaceMatrix(const glm::mat4& mat4) { lightSpaceMatrix = mat4; }
  glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }
  const float* getLightSpaceMatrixPTR() const { return glm::value_ptr(lightSpaceMatrix); }
  void setLightVector(const glm::vec4& vec4) { lightVector = vec4; }
  glm::vec4 getLightVector() const { return lightVector; }
  const float* getLightVectorPTR() const { return glm::value_ptr(lightVector); }
  glm::vec4 getLightCoefficients() const { return lightCoefficients; }
  const float* getLightCoefficientsPTR() const { return glm::value_ptr(lightCoefficients); }

  CONSTEXPR_VIRTUAL virtual const char* getTypeName() const = 0;
  CONSTEXPR_VIRTUAL virtual LightType getType() const = 0;

  virtual void update(const glm::mat4&) {}

 protected:
  glm::vec4 lightCoefficients;
  glm::vec4 lightVector;
  glm::mat4 lightSpaceMatrix;
};
using LightPTR = std::unique_ptr<Light>;
}  // namespace graphics::light
