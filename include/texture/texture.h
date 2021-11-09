#pragma once
#include <array>
#include <iostream>
#include <unordered_map>

#include <glad/gl.h>
#include "utils.h"

namespace graphics::texture {
constexpr GLenum getColorFormat(int channels) {
  switch (channels) {
    case 1: return GL_RED;
    case 2: return GL_RG;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default:
      std::cout << "Unknown color format!" << std::endl;
      std::cout << "Guess color: RGB." << std::endl;
      return GL_RGB;
  }
}
class Texture {
 public:
  MOVE_ONLY(Texture)
  Texture() noexcept;
  virtual ~Texture();
  CONSTEXPR_VIRTUAL virtual const char* getTypeName() const = 0;
  CONSTEXPR_VIRTUAL virtual GLenum getType() const = 0;
  void bind(GLuint index = 0) const;

 protected:
  static std::array<std::unordered_map<GLenum, GLuint>, 16> currentBinding;
  static GLenum currentActiveTextureUnit;
  GLuint handle;
};
}  // namespace graphics::texture
