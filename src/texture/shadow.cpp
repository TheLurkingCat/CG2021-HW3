#include "texture/shadow.h"

namespace graphics::texture {
Framebuffer::Framebuffer() noexcept : handle(0) { glGenFramebuffers(1, &handle); }

Framebuffer::~Framebuffer() { glGenFramebuffers(1, &handle); }

void Framebuffer::bind() const { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle); }

ShadowMap::ShadowMap(unsigned int size) noexcept : shadowSize(size) {
  GLfloat borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  bind(15);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  framebuffer.bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, handle, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

}  // namespace graphics::texture
