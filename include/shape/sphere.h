#pragma once
#include <memory>
#include <utility>
#include <vector>

#include <glad/gl.h>

#include "buffer/buffer.h"
#include "buffer/vertexarray.h"
#include "shape.h"

namespace graphics::shape {
class Sphere final : public Shape {
 public:
  Sphere();
  Sphere(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
  void draw() const override;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Sphere"; }
  CONSTEXPR_VIRTUAL ShapeType getType() const override { return ShapeType::Sphere; }
  template <typename... Args>
  static std::unique_ptr<Sphere> make_unique(Args&&... args) {
    return std::make_unique<Sphere>(std::forward<Args>(args)...);
  }
  static void generateVertices(std::vector<GLfloat>& vertex,
                               std::vector<GLuint>& index,
                               int stack = 18,
                               int slice = 36);

 private:
  std::shared_ptr<buffer::VertexArray> vao;
  std::shared_ptr<buffer::ArrayBuffer> vbo;
  std::shared_ptr<buffer::ElementArrayBuffer> ebo;

  static std::weak_ptr<buffer::VertexArray> vao_weak;
  static std::weak_ptr<buffer::ArrayBuffer> vbo_weak;
  static std::weak_ptr<buffer::ElementArrayBuffer> ebo_weak;
};
using SpherePTR = std::unique_ptr<Sphere>;
};  // namespace graphics::shape
