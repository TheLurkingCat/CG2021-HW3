#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include <glad/gl.h>
#include <glm/fwd.hpp>

#include "shader.h"
#include "utils.h"
namespace graphics::shader {
class ShaderProgram final {
 public:
  MOVE_ONLY(ShaderProgram)
  ShaderProgram() noexcept;
  ~ShaderProgram();
  void attach(Shader* shader);
  template <class... Shaders>
  void attach(Shader* shader, Shaders... shaders) {
    attach(shader);
    attach(std::forward<Shaders>(shaders)...);
  }
  void detach(Shader* shader);
  template <class... Shaders>
  void detach(Shader* shader, Shaders... shaders) {
    detach(shader);
    detach(std::forward<Shaders>(shaders)...);
  }

  void link();
  bool checkLinkState() const;

  GLuint getHandle() const;
  void use() const;

  GLint getUniformLocation(const char* name) const;
  GLuint getUniformBlockIndex(const char* name) const;
  void uniformBlockBinding(const char* name, GLuint binding) const;
  void uniformBlockBinding(GLuint index, GLuint binding) const;
  void setUniform(const char* name, GLint i1);
  void setUniform(GLint location, GLint i1);

 private:
  static GLuint currentBinding;
  bool isLinked;
  GLuint handle;
  mutable std::unordered_map<std::string, GLint> uniforms;
  mutable std::unordered_map<std::string, GLint> uniformBlocks;
};
}  // namespace graphics::shader
