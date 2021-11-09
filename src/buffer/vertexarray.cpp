#include "buffer/vertexarray.h"

namespace graphics::buffer {
VertexArray::VertexArray() { glGenVertexArrays(1, &handle); }
VertexArray::~VertexArray() { glDeleteVertexArrays(1, &handle); }

void VertexArray::enable(int index) { glEnableVertexAttribArray(index); }
void VertexArray::setAttributePointer(int index, int size, int stride, int offset) {
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat),
                        reinterpret_cast<GLvoid *>(offset * sizeof(GLfloat)));
}
void VertexArray::bind() { glBindVertexArray(handle); }
}  // namespace graphics::buffer
