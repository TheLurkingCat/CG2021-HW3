#pragma once
#include <glad/gl.h>

#include "utils.h"
namespace graphics::buffer {
class VertexArray {
 public:
  MOVE_ONLY(VertexArray)
  VertexArray();
  ~VertexArray();

  void enable(int index);
  void setAttributePointer(int index, int size, int stride, int offset);
  void bind();

 private:
  GLuint handle;
};
}  // namespace graphics::buffer
