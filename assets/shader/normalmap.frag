#version 330 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
  vec3 position;
  vec3 lightDirection;
  vec2 textureCoordinate;
  flat vec3 viewPosition;
} fs_in;

// RGB contains the color
uniform sampler2D diffuseTexture;
// RGB contains the normal
uniform sampler2D normalTexture;

void main() {
  // Query diffuse texture
  vec3 diffuseColor = texture(diffuseTexture, fs_in.textureCoordinate).rgb;
  // Ambient intensity
  float ambient = 0.1;
  float diffuse = 0.1;
  float specular = 0.1;
  // TODO: Blinn-Phong shading
  //   1. Query normalTexture using to find this fragment's normal
  //   2. Convert the value from RGB [0, 1] to normal [-1, 1], this will be inverse of what you do in calculatenormal.frag's output.
  //   3. Remember to NORMALIZE it again.
  //   4. Use Blinn-Phong shading here with parameters ks = kd = 0.75

  vec3 normal = texture(normalTexture, fs_in.textureCoordinate).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  vec3 viewDirection = normalize(fs_in.viewPosition - fs_in.position);
  vec3 halfwayDirection = normalize(viewDirection - fs_in.lightDirection);
  // Diffuse intensity
  diffuse = 0.75 * max(dot(normal, -fs_in.lightDirection), 0.0);
  // Specular intensity (Blinn-Phong)
  specular = 0.75 * pow(max(dot(normal, halfwayDirection), 0.0), 8.0);

  float lighting = ambient + diffuse + specular;
  FragColor = vec4(lighting * diffuseColor, 1.0);
}
