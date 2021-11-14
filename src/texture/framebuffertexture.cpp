#include "texture/framebuffertexture.h"
#include <vector>
namespace graphics::texture {
Framebuffer::Framebuffer() noexcept : handle(0) { glGenFramebuffers(1, &handle); }

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &handle); }

void Framebuffer::setBuffers(std::initializer_list<GLenum> drawbuffers, GLenum readbuffer) const {
  bind();
  std::vector<GLenum> attachments{drawbuffers};
  glDrawBuffers(attachments.size(), attachments.data());
  glReadBuffer(readbuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Framebuffer::bind() const { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle); }

FramebufferTexture::FramebufferTexture(unsigned int size) noexcept : textureSize{size} {}

void FramebufferTexture::attachtoFramebuffer(Framebuffer* fbo, GLenum attachment) const {
  fbo->bind();
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, handle, 0);
  GLuint clearColor[4] = {0, 0, 0, 0};
  glClearBufferuiv(GL_COLOR, 0, clearColor);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

DepthMap::DepthMap(unsigned int size, GLenum internalColor) noexcept : FramebufferTexture(size) {
  GLfloat borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  bind(15);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexImage2D(GL_TEXTURE_2D, 0, internalColor, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}

ColorMap::ColorMap(unsigned int size, GLenum internalColor) noexcept : FramebufferTexture(size) {
  bind(15);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, internalColor, size, size, 0, GL_RGB, GL_FLOAT, NULL);
}
}  // namespace graphics::texture
