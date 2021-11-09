#include "shape/cube.h"
namespace graphics::shape {

std::weak_ptr<buffer::VertexArray> Cube::vao_weak;
std::weak_ptr<buffer::ArrayBuffer> Cube::vbo_weak;
std::weak_ptr<buffer::ElementArrayBuffer> Cube::ebo_weak;

Cube::Cube() {
  if ((ebo = ebo_weak.lock())) {
    // Already have vertex data.
    vao = vao_weak.lock();
    vbo = vbo_weak.lock();
  } else {
    // First object.
    std::vector<GLfloat> vertices;
    std::vector<GLubyte> indices;
    generateVertices(vertices, indices);

    vao_weak = vao = std::make_shared<buffer::VertexArray>();
    vbo_weak = vbo = std::make_shared<buffer::ArrayBuffer>();
    ebo_weak = ebo = std::make_shared<buffer::ElementArrayBuffer>();

    vbo->allocate_load(vertices.size() * sizeof(GLfloat), vertices.data());
    ebo->allocate_load(indices.size() * sizeof(GLubyte), indices.data());

    vao->bind();
    vbo->bind();
    ebo->bind();

    vao->enable(0);
    vao->setAttributePointer(0, 3, 6, 0);
    vao->enable(1);
    vao->setAttributePointer(1, 3, 6, 3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

Cube::Cube(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) {
  vao = std::make_shared<buffer::VertexArray>();
  vbo = std::make_shared<buffer::ArrayBuffer>();
  ebo = std::make_shared<buffer::ElementArrayBuffer>();

  vbo->allocate_load(vertices.size() * sizeof(GLfloat), vertices.data());
  ebo->allocate_load(indices.size() * sizeof(GLuint), indices.data());

  vao->bind();
  vbo->bind();
  ebo->bind();

  vao->enable(0);
  vao->setAttributePointer(0, 3, 6, 0);
  vao->enable(1);
  vao->setAttributePointer(1, 3, 6, 3);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::draw() const {
  vao->bind();
  GLsizei indexCount = static_cast<GLsizei>(ebo->getSize() / sizeof(GLubyte));
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, nullptr);
  glBindVertexArray(0);
}

void Cube::generateVertices(std::vector<GLfloat>& vertices, std::vector<GLubyte>& indices) {
  vertices.reserve(192);
  indices.reserve(36);
  vertices = std::vector<GLfloat>{// Front
                                  1, 1, 1, 0, 0, 1, -1, 1, 1, 0, 0, 1, -1, -1, 1, 0, 0, 1, 1, -1, 1, 0, 0, 1,
                                  // Right
                                  1, 1, 1, 1, 0, 0, 1, -1, 1, 1, 0, 0, 1, -1, -1, 1, 0, 0, 1, 1, -1, 1, 0, 0,
                                  // Top
                                  1, 1, 1, 0, 1, 0, 1, 1, -1, 0, 1, 0, -1, 1, -1, 0, 1, 0, -1, 1, 1, 0, 1, 0,
                                  // Left
                                  -1, 1, 1, -1, 0, 0, -1, 1, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, -1, -1, 1, -1, 0, 0,
                                  // Bottom
                                  -1, -1, -1, 0, -1, 0, 1, -1, -1, 0, -1, 0, 1, -1, 1, 0, -1, 0, -1, -1, 1, 0, -1, 0,
                                  // Back
                                  1, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, -1, -1, 1, -1, 0, 0, -1, 1, 1, -1, 0, 0, -1};

  indices = std::vector<GLubyte>{0,  1,  2,  2,  3,  0,    // front
                                 4,  5,  6,  6,  7,  4,    // right
                                 8,  9,  10, 10, 11, 8,    // top
                                 12, 13, 14, 14, 15, 12,   // left
                                 16, 17, 18, 18, 19, 16,   // bottom
                                 20, 21, 22, 22, 23, 20};  // back
}
}  // namespace graphics::shape
