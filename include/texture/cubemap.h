#pragma once
#include "texture.h"

#include <glm/fwd.hpp>
namespace graphics::texture {
class TextureCubeMap : public Texture {
 public:
  void fromFile(const utils::fs::path& posx,
                const utils::fs::path& negx,
                const utils::fs::path& posy,
                const utils::fs::path& negy,
                const utils::fs::path& posz,
                const utils::fs::path& negz) const;

  void fromColor(const glm::vec4& posx,
                 const glm::vec4& negx,
                 const glm::vec4& posy,
                 const glm::vec4& negy,
                 const glm::vec4& posz,
                 const glm::vec4& negz) const;
  CONSTEXPR_VIRTUAL const char* getTypeName() const override { return "TextureCubeMap"; }
  CONSTEXPR_VIRTUAL GLenum getType() const override { return GL_TEXTURE_CUBE_MAP; }
};
}  // namespace graphics::texture
