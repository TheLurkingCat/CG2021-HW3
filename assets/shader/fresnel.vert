#version 330 core
layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 normal_in;

out VS_OUT {
  vec3 position;
  vec3 normal;
  flat vec3 viewPosition;
} vs_out;

layout (std140) uniform model {
  // Model matrix
  mat4 modelMatrix;
  // inverse(transpose(model)), precalculate using CPU for efficiency
  mat4 normalMatrix;
};

layout (std140) uniform camera {
  // Projection * View matrix
  mat4 viewProjectionMatrix;
  // Position of the camera
  vec4 viewPosition;
};

void main() {
  vs_out.position = vec3(modelMatrix * vec4(position_in, 1.0));
  vs_out.normal = normalize(mat3(normalMatrix) * normal_in);
  vs_out.viewPosition = viewPosition.xyz;
  gl_Position = viewProjectionMatrix * modelMatrix * vec4(position_in, 1.0);
}
