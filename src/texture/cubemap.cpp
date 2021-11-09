#include "texture/cubemap.h"
#include <array>

#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>
namespace graphics::texture {
void TextureCubeMap::fromFile(const utils::fs::path& posx,
                              const utils::fs::path& negx,
                              const utils::fs::path& posy,
                              const utils::fs::path& negy,
                              const utils::fs::path& posz,
                              const utils::fs::path& negz) const {
  std::array<const utils::fs::path, 6> filenames{posx, negx, posy, negy, posz, negz};
  int width, height, nChannels;
  stbi_set_flip_vertically_on_load(1);
  bind(15);
  for (int i = 0; i < 6; ++i) {
    stbi_uc* data = stbi_load(filenames[i].string().c_str(), &width, &height, &nChannels, STBI_default);
    if (data == nullptr) THROW_EXCEPTION(std::runtime_error, "Failed to load texture file");
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, getColorFormat(nChannels),
                 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void TextureCubeMap::fromColor(const glm::vec4& posx,
                               const glm::vec4& negx,
                               const glm::vec4& posy,
                               const glm::vec4& negy,
                               const glm::vec4& posz,
                               const glm::vec4& negz) const {
  std::array<const glm::vec4, 6> colors{posx, negx, posy, negy, posz, negz};
  bind(15);
  for (int i = 0; i < 6; ++i) {
    glm::u8vec4 colorByte = glm::round(255.0f * colors[i]);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glm::value_ptr(colorByte));
  }
}
}  // namespace graphics::texture
