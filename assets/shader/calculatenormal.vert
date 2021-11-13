#version 330 core
layout(location = 0) in vec3 position_in;
// We will not use normal input in this part
layout(location = 1) in vec3 normal_in;
// We will not use texture coordinate input in this part
layout(location = 2) in vec2 textureCoordinate_in;

void main() {
  gl_Position = vec4(position_in.x, -position_in.z, 0.0, 1.0);
}
