#pragma once
#include <memory>
#include <utility>
#include <vector>

#include <glad/gl.h>

#include "buffer/buffer.h"
#include "buffer/vertexarray.h"
#include "shape.h"

namespace graphics::shape {
class Plane final : public Shape {
 public:
  Plane();
  Plane(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
  static void generateVertices(std::vector<GLfloat>& vertex,
                               std::vector<GLuint>& index,
                               int subdivision = 1,
                               float width = 1,
                               float height = 1,
                               bool scaleTexture = true);

  void draw() const override;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Plane"; }
  CONSTEXPR_VIRTUAL ShapeType getType() const override { return ShapeType::Plane; }
  template <typename... Args>
  static std::unique_ptr<Plane> make_unique(Args&&... args) {
    return std::make_unique<Plane>(std::forward<Args>(args)...);
  }

 private:
  std::shared_ptr<buffer::VertexArray> vao;
  std::shared_ptr<buffer::ArrayBuffer> vbo;
  std::shared_ptr<buffer::ElementArrayBuffer> ebo;

  static std::weak_ptr<buffer::VertexArray> vao_weak;
  static std::weak_ptr<buffer::ArrayBuffer> vbo_weak;
  static std::weak_ptr<buffer::ElementArrayBuffer> ebo_weak;
};
using PlanePTR = std::unique_ptr<Plane>;
};  // namespace graphics::shape
