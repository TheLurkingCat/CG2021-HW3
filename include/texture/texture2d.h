#pragma once
#include "texture/texture.h"

#include <glm/fwd.hpp>
namespace graphics::texture {
class Texture2D : public Texture {
 public:
  void fromFile(const utils::fs::path& path) const;
  void fromColor(const glm::vec4& color) const;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "Texture2D"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TEXTURE_2D; }
};
}  // namespace graphics::texture
