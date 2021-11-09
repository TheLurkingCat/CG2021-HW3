#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"
namespace graphics::shape {
enum class ShapeType : uint8_t { Cube, Sphere, Plane };
class Shape {
 public:
  Shape() noexcept : modelMatrix(1), normalMatrix(1) {}
  virtual ~Shape() = default;
  virtual void draw() const = 0;
  CONSTEXPR_VIRTUAL virtual const char* getTypeName() const = 0;
  CONSTEXPR_VIRTUAL virtual ShapeType getType() const = 0;

  void setModelMatrix(const glm::mat4& _modelMatrix) {
    modelMatrix = _modelMatrix;
    normalMatrix = glm::mat4(glm::inverseTranspose(glm::mat3(modelMatrix)));
  }

  glm::mat4 getModelMatrix() const { return modelMatrix; }
  const float* getModelMatrixPTR() const { return glm::value_ptr(modelMatrix); }
  glm::mat4 getNormalMatrix() const { return normalMatrix; }
  const float* getNormalMatrixPTR() const { return glm::value_ptr(normalMatrix); }

 private:
  glm::mat4 modelMatrix;
  glm::mat4 normalMatrix;
};
using ShapePTR = std::unique_ptr<Shape>;
}  // namespace graphics::shape
