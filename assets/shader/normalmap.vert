#version 330 core
layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 textureCoordinate_in;
layout(location = 3) in vec3 tangent_in;
layout(location = 4) in vec3 bitangent_in;

out VS_OUT {
  vec3 position;
  vec3 lightDirection;
  vec2 textureCoordinate;
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
  // Direction of light, hard coded here for convinience.
  // const vec3 lightDirection = normalize(vec3(0, 0, -1));
  const vec3 lightDirection = normalize(vec3(-11.1, -24.9, 14.8));
  // TODO:
  //   1. Calculate the inverse of tangent space transform matrix (TBN matrix)
  //   2. Transform light direction, viewPosition, and position to the tangent space.
  vec3 T = normalize(vec3(normalMatrix * vec4(tangent_in,   0.0)));
  vec3 B = normalize(vec3(normalMatrix * vec4(bitangent_in, 0.0)));
  vec3 N = normalize(vec3(normalMatrix * vec4(normal_in,    0.0)));
  // This is "inverse" of rotation matrix
  mat3 TBN = transpose(mat3(T, B, N));
  vs_out.lightDirection = TBN * lightDirection;
  vs_out.viewPosition = TBN * viewPosition.xyz;
  vs_out.position =  TBN * vec3(modelMatrix * vec4(position_in, 1.0));
  vs_out.textureCoordinate = textureCoordinate_in;
  gl_Position = viewProjectionMatrix * modelMatrix * vec4(position_in, 1.0);
}
