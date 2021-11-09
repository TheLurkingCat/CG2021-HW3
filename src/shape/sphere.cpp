#include "shape/sphere.h"
namespace graphics::shape {

std::weak_ptr<buffer::VertexArray> Sphere::vao_weak;
std::weak_ptr<buffer::ArrayBuffer> Sphere::vbo_weak;
std::weak_ptr<buffer::ElementArrayBuffer> Sphere::ebo_weak;

Sphere::Sphere() {
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
    vao->setAttributePointer(0, 3, 8, 0);
    vao->enable(1);
    vao->setAttributePointer(1, 3, 8, 3);
    vao->enable(2);
    vao->setAttributePointer(2, 2, 8, 6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

Sphere::Sphere(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) {
  vao = std::make_shared<buffer::VertexArray>();
  vbo = std::make_shared<buffer::ArrayBuffer>();
  ebo = std::make_shared<buffer::ElementArrayBuffer>();

  vbo->allocate_load(vertices.size() * sizeof(GLfloat), vertices.data());
  ebo->allocate_load(indices.size() * sizeof(GLuint), indices.data());

  vao->bind();
  vbo->bind();
  ebo->bind();

  vao->enable(0);
  vao->setAttributePointer(0, 3, 8, 0);
  vao->enable(1);
  vao->setAttributePointer(1, 3, 8, 3);
  vao->enable(2);
  vao->setAttributePointer(2, 2, 8, 6);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sphere::draw() const {
  vao->bind();
  GLsizei indexCount = static_cast<GLsizei>(ebo->getSize() / sizeof(GLuint));
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Sphere::generateVertices(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, int stack, int slice) {
  // See http://www.songho.ca/opengl/gl_sphere.html#sphere if you don't know how to create a sphere.
  vertices.reserve(8 * (stack + 1) * (slice + 1));
  indices.reserve(6 * stack * slice);

  float x, y, z, xy;  //  position
  float s, t;         //  texCoord

  float sectorStep = glm::two_pi<float>() / slice;
  float stackStep = glm::pi<float>() / stack;
  float sectorAngle, stackAngle;

  for (int i = 0; i <= stack; ++i) {
    stackAngle = static_cast<float>(glm::half_pi<float>() - i * stackStep);  // [pi/2, -pi/2]
    xy = cosf(stackAngle);                                                   // r * cos(u)
    z = sinf(stackAngle);                                                    // r * sin(u)

    for (int j = 0; j <= slice; ++j) {
      sectorAngle = j * sectorStep;  // [0, 2pi]

      x = xy * cosf(sectorAngle);  // r * cos(u) * cos(v)
      y = xy * sinf(sectorAngle);  // r * cos(u) * sin(v)

      // vertex tex coord (s, t) range between [0, 1]
      s = static_cast<float>(j) / slice;
      t = static_cast<float>(i) / stack;

      vertices.insert(vertices.end(), {x, y, z, x, y, z, s, t});
    }
  }

  unsigned int k1, k2;  // EBO index
  for (int i = 0; i < stack; ++i) {
    k1 = i * (slice + 1);  // beginning of current stack
    k2 = k1 + slice + 1;   // beginning of next stack
    for (int j = 0; j < slice; ++j, ++k1, ++k2) {
      if (i != 0) {
        indices.insert(indices.end(), {k1, k2, k1 + 1});
      }
      // k1+1 => k2 => k2+1
      if (i != (stack - 1)) {
        indices.insert(indices.end(), {k1 + 1, k2, k2 + 1});
      }
    }
  }
}
}  // namespace graphics::shape
