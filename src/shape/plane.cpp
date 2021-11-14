#include "shape/plane.h"
namespace graphics::shape {

std::weak_ptr<buffer::VertexArray> Plane::vao_weak;
std::weak_ptr<buffer::ArrayBuffer> Plane::vbo_weak;
std::weak_ptr<buffer::ElementArrayBuffer> Plane::ebo_weak;

Plane::Plane() {
  if ((ebo = ebo_weak.lock())) {
    // Already have vertex data.
    vao = vao_weak.lock();
    vbo = vbo_weak.lock();
  } else {
    // First object.
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    generateVertices(vertices, indices);

    vao_weak = vao = std::make_shared<buffer::VertexArray>();
    vbo_weak = vbo = std::make_shared<buffer::ArrayBuffer>();
    ebo_weak = ebo = std::make_shared<buffer::ElementArrayBuffer>();

    vbo->allocate_load(vertices.size() * sizeof(GLfloat), vertices.data());
    ebo->allocate_load(indices.size() * sizeof(GLuint), indices.data());

    vao->bind();
    vbo->bind();
    ebo->bind();

    vao->enable(0);
    vao->setAttributePointer(0, 3, 14, 0);
    vao->enable(1);
    vao->setAttributePointer(1, 3, 14, 3);
    vao->enable(2);
    vao->setAttributePointer(2, 2, 14, 6);
    vao->enable(3);
    vao->setAttributePointer(3, 3, 14, 8);
    vao->enable(4);
    vao->setAttributePointer(4, 3, 14, 11);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

Plane::Plane(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) {
  vao = std::make_shared<buffer::VertexArray>();
  vbo = std::make_shared<buffer::ArrayBuffer>();
  ebo = std::make_shared<buffer::ElementArrayBuffer>();

  vbo->allocate_load(vertices.size() * sizeof(GLfloat), vertices.data());
  ebo->allocate_load(indices.size() * sizeof(GLuint), indices.data());

  vao->bind();
  vbo->bind();
  ebo->bind();

  vao->enable(0);
  vao->setAttributePointer(0, 3, 14, 0);
  vao->enable(1);
  vao->setAttributePointer(1, 3, 14, 3);
  vao->enable(2);
  vao->setAttributePointer(2, 2, 14, 6);
  vao->enable(3);
  vao->setAttributePointer(3, 3, 14, 8);
  vao->enable(4);
  vao->setAttributePointer(4, 3, 14, 11);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Plane::draw() const {
  if (preDrawCallback) preDrawCallback();
  vao->bind();
  GLsizei indexCount = static_cast<GLsizei>(ebo->getSize() / sizeof(GLuint));
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  if (postDrawCallback) postDrawCallback();
}

void Plane::generateVertices(std::vector<GLfloat>& vertices,
                             std::vector<GLuint>& indices,
                             int subdivision,
                             float width,
                             float height,
                             bool scaleTexture) {
  vertices.reserve((subdivision + 1) * (subdivision + 1) * 8);
  indices.reserve((subdivision) * (2 * subdivision + 3));

  float wStep = 2.0f * width / subdivision;
  float hStep = 2.0f * height / subdivision;
  float wTexStep = scaleTexture ? 1.0f / subdivision : width / subdivision;
  float hTexStep = scaleTexture ? 1.0f / subdivision : height / subdivision;
  for (int i = 0; i < subdivision + 1; ++i) {
    for (int j = 0; j < subdivision + 1; ++j) {
      vertices.insert(vertices.end(), {-width + j * wStep, 0, -height + i * hStep, 0, 1, 0});
      if (scaleTexture)
        vertices.insert(vertices.end(), {0 + j * wTexStep, 1 - i * hTexStep});
      else
        vertices.insert(vertices.end(), {0 + j * wTexStep, height - i * hTexStep});
      vertices.insert(vertices.end(), {1, 0, 0, 0, 0, -1});
    }
  }

  for (int i = 0; i < subdivision; ++i) {
    int offset = i * (subdivision + 1);
    for (int j = 0; j < subdivision; ++j) {
      indices.emplace_back(offset + j);
      indices.emplace_back(offset + j + subdivision + 1);
      indices.emplace_back(offset + j + 1);

      indices.emplace_back(offset + j + 1);
      indices.emplace_back(offset + j + subdivision + 1);
      indices.emplace_back(offset + j + subdivision + 2);
    }
  }
}
}  // namespace graphics::shape
