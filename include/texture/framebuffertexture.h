#pragma once
#include "texture/texture.h"

namespace graphics::texture {

class Framebuffer {
 public:
  MOVE_ONLY(Framebuffer)
  Framebuffer() noexcept;
  ~Framebuffer();

  void bind() const;
  void setBuffers(std::initializer_list<GLenum> drawbuffers, GLenum readbuffer) const;

 private:
  GLuint handle;
};

class FramebufferTexture : public Texture {
 public:
  unsigned int getSize() const { return textureSize; }
  void attachtoFramebuffer(Framebuffer* fbo, GLenum attachment) const;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Texture2D (With framebuffer)"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TEXTURE_2D; }

 protected:
  explicit FramebufferTexture(unsigned int size) noexcept;
  unsigned int textureSize;
};

class DepthMap final : public FramebufferTexture {
 public:
  explicit DepthMap(unsigned int size, GLenum internalColor = GL_DEPTH_COMPONENT) noexcept;
};

class ColorMap final : public FramebufferTexture {
 public:
  ColorMap(unsigned int size, GLenum internalColor = GL_RGB) noexcept;
};
}  // namespace graphics::texture
