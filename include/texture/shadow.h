#pragma once
#include "texture/texture.h"

namespace graphics::texture {

class Framebuffer {
 public:
  MOVE_ONLY(Framebuffer)
  Framebuffer() noexcept;
  ~Framebuffer();

  void bind() const;

 private:
  GLuint handle;
};

class ShadowMap final : public Texture {
 public:
  ShadowMap(unsigned int size) noexcept;
  unsigned int getSize() const { return shadowSize; }
  void bindFramebuffer() const noexcept { framebuffer.bind(); }
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Texture2D (shadow map)"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TEXTURE_2D; }

 private:
  Framebuffer framebuffer;
  const unsigned int shadowSize;
};
}  // namespace graphics::texture
