#pragma once
#include <memory>
#include <utility>
#include <vector>

#include <glad/gl.h>

#include "buffer/buffer.h"
#include "buffer/vertexarray.h"
#include "shape.h"

namespace graphics::shape {
class Cube final : public Shape {
 public:
  Cube();
  Cube(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
  static void generateVertices(std::vector<GLfloat>& vertex, std::vector<GLubyte>& index);

  void draw() const override;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Cube"; }
  CONSTEXPR_VIRTUAL ShapeType getType() const override { return ShapeType::Cube; }
  template <typename... Args>
  static std::unique_ptr<Cube> make_unique(Args&&... args) {
    return std::make_unique<Cube>(std::forward<Args>(args)...);
  }

 private:
  std::shared_ptr<buffer::VertexArray> vao;
  std::shared_ptr<buffer::ArrayBuffer> vbo;
  std::shared_ptr<buffer::ElementArrayBuffer> ebo;

  static std::weak_ptr<buffer::VertexArray> vao_weak;
  static std::weak_ptr<buffer::ArrayBuffer> vbo_weak;
  static std::weak_ptr<buffer::ElementArrayBuffer> ebo_weak;
};
using CubePTR = std::unique_ptr<Cube>;
};  // namespace graphics::shape
