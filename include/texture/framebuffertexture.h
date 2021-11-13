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

class FramebufferTexture : public Texture {
 public:
  unsigned int getSize() const { return textureSize; }
  void bindFramebuffer() const noexcept { framebuffer.bind(); }
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Texture2D (With framebuffer)"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TEXTURE_2D; }

 protected:
  explicit FramebufferTexture(unsigned int size) noexcept;
  Framebuffer framebuffer;
  unsigned int textureSize;
};

class ShadowMap final : public FramebufferTexture {
 public:
  explicit ShadowMap(unsigned int size) noexcept;
};

class NormalMap final : public FramebufferTexture {
 public:
  explicit NormalMap(unsigned int size) noexcept;
};
}  // namespace graphics::texture
