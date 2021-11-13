#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "shader/program.h"
#include "shape/shape.h"
#include "texture/texture.h"
#include "utils.h"

namespace utils {
struct Mesh {
  MOVE_ONLY(Mesh)
  Mesh(graphics::shape::Shape* _shape,
       graphics::shader::ShaderProgram* _program,
       std::vector<graphics::texture::Texture*> _textures) :
      shape{std::move(_shape)}, program{std::move(_program)}, textures{std::move(_textures)} {}

  void draw() {
    program->use();
    for (int i = 0; i < textures.size(); ++i)
      if (textures[i]) textures[i]->bind(i);
    shape->draw();
  }

  graphics::shape::Shape* shape;
  graphics::shader::ShaderProgram* program;
  std::vector<graphics::texture::Texture*> textures;
};
}  // namespace utils
