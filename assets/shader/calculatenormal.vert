#version 330 core
layout(location = 0) in vec3 position_in;

void main() {
  gl_Position = vec4(position_in.x, -position_in.z, 0.0, 1.0);
}
