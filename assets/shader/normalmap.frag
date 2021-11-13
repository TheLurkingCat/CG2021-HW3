#version 330 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 textureCoordinate;
  flat vec3 viewPosition;
} fs_in;

uniform sampler2D diffuseTexture;
// RGB contains the normal
uniform sampler2D normalTexture;

void main() {
  vec3 normal = texture(normalTexture, fs_in.textureCoordinate).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  // vec3 normal = fs_in.normal;
  // Direction of light, hard coded here for convinience.
  vec3 lightDirection = normalize(vec3(-11.1, -24.9, 14.8));
  vec3 viewDirection = normalize(fs_in.viewPosition - fs_in.position);
  float attenuation = 0.65;

  vec3 halfwayDirection = normalize(lightDirection + viewDirection);
  // Ambient intensity
  float ambient = 0.1;
  // Diffuse intensity
  float normalDotLight = dot(normal, lightDirection);
  float diffuse = 0.75 * max(normalDotLight, 0.0);
  // Specular intensity (Blinn-Phong)
  float specular = 0.75 * pow(max(dot(normal, halfwayDirection), 0.0), 8.0);
  float lighting = (ambient + attenuation * (diffuse + specular));
  vec3 diffuseColor = texture(diffuseTexture, fs_in.textureCoordinate).rgb;
  FragColor = vec4(lighting * diffuseColor, 1.0);
}
