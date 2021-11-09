#include "shader/shader.h"
#include <fstream>
#include <string>
namespace {
std::string readFile(const utils::fs::path& filename) {
  std::ifstream shaderFile(filename);
  if (!shaderFile) {
    std::string err = "Cannot open shader file: " + filename.string();
    if (glDebugMessageInsert) {
      glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, -1,
                           err.c_str());
    } else {
      puts(err.c_str());
    }
  }
  auto shaderCode = std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());
  return shaderCode;
}
}  // namespace

namespace graphics::shader {

Shader::Shader(GLenum shaderType) noexcept : handle(glCreateShader(shaderType)) {}
Shader::~Shader() { glDeleteShader(handle); }

GLuint Shader::getHandle() const { return handle; }

bool Shader::checkCompileState() const {
  GLint success;
  GLchar infoLog[1024];
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(handle, 1024, nullptr, infoLog);
    if (glDebugMessageInsert) {
      glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, -1, infoLog);
    } else {
      puts("Shader compilation error!");
      puts(infoLog);
    }
  }
  return success;
}

void Shader::fromFile(const utils::fs::path& filename) const { this->fromString(readFile(filename)); }

void Shader::fromString(const std::string& shaderCode) const {
  auto shaderCodePointer = shaderCode.c_str();
  glShaderSource(handle, 1, &shaderCodePointer, nullptr);
  glCompileShader(handle);
}
}  // namespace graphics::shader
