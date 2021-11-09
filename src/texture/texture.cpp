#include "texture/texture.h"

namespace graphics::texture {
std::array<std::unordered_map<GLenum, GLuint>, 16> Texture::currentBinding;
GLenum Texture::currentActiveTextureUnit = GL_TEXTURE0;

Texture::Texture() noexcept : handle(0) { glGenTextures(1, &handle); }

Texture::~Texture() { glDeleteTextures(1, &handle); }

void Texture::bind(GLuint index) const {
  GLenum textureUnit = GL_TEXTURE0 + index;
  if (currentActiveTextureUnit != textureUnit) {
    glActiveTexture(textureUnit);
    currentActiveTextureUnit = textureUnit;
  }
  GLenum textureType = getType();
  auto& currentHandle = currentBinding[index][textureType];
  if (handle != currentHandle) {
    glBindTexture(textureType, handle);
    currentHandle = handle;
  }
}
}  // namespace graphics::texture
