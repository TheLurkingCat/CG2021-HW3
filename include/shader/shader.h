#pragma once
#include <string>

#include <glad/gl.h>

#include "utils.h"
namespace graphics::shader {
class Shader {
 public:
  MOVE_ONLY(Shader)
  explicit Shader(GLenum shaderType) noexcept;
  virtual ~Shader();
  CONSTEXPR_VIRTUAL virtual const char* getTypeName() const = 0;
  CONSTEXPR_VIRTUAL virtual GLenum getType() const = 0;
  GLuint getHandle() const;
  bool checkCompileState() const;
  void fromFile(const utils::fs::path& filename) const;
  void fromString(const std::string& shadercode) const;

 protected:
  GLuint handle;
};

class ComputeShader final : public Shader {
 public:
  ComputeShader() noexcept : Shader(GL_COMPUTE_SHADER) {}
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Compute shader"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_COMPUTE_SHADER; }
};

class VertexShader final : public Shader {
 public:
  VertexShader() noexcept : Shader(GL_VERTEX_SHADER) {}
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Vertex shader"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_VERTEX_SHADER; }
};

class TessControlShader final : public Shader {
 public:
  TessControlShader() noexcept : Shader(GL_TESS_CONTROL_SHADER) {}
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Tessellation control shader"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TESS_CONTROL_SHADER; }
};

class TessEvaluationShader final : public Shader {
 public:
  TessEvaluationShader() noexcept : Shader(GL_TESS_EVALUATION_SHADER) {}
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Tessellation evaluation shader"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TESS_EVALUATION_SHADER; }
};

class GeometryShader final : public Shader {
 public:
  GeometryShader() noexcept : Shader(GL_GEOMETRY_SHADER) {}
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Geometry shader"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_GEOMETRY_SHADER; }
};

class FragmentShader final : public Shader {
 public:
  MOVE_ONLY(FragmentShader)
  FragmentShader() noexcept : Shader(GL_FRAGMENT_SHADER) {}
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Fragment shader"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_FRAGMENT_SHADER; }
};
}  // namespace graphics::shader
