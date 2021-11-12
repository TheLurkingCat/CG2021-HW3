#version 330 core
layout (location = 0) in vec3 position_in;
// We will not use normal input in this part
layout(location = 1) in vec3 normal_in;
// We will not use texture coordinate input in this part
layout(location = 2) in vec2 textureCoordinate_in;

out vec3 textureCoordinate;

// We need to cancel translation in view matrix
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = (projection * mat4(mat3(view)) * vec4(position_in, 1.0)).xyww;
  textureCoordinate = position_in;
}
