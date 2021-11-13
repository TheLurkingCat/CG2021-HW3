#version 330 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
  vec3 position;
  vec3 normal;
  flat vec3 viewPosition;
} fs_in;

uniform samplerCube skybox;

uniform float fresnelBias;
uniform float fresnelScale;
uniform float fresnelPower;

void main() {
  // Refractive index of R, G, and B respectively
  vec3 Eta = vec3(1/ 1.39, 1 / 1.44, 1 / 1.47);
  // TODO: fresnel reflection and refraction
  // Hint:
  //   1. https://developer.download.nvidia.com/CgTutorial/cg_tutorial_chapter07.html
  //   2. You should query the texture for R, G, and B values respectively to create dispersion effect.
  //   3. You should use those uniform variables in the equation(1).
  // Note:
  //   1. Above link is not GLSL you just check the concept.
  //   2. We use the empirical approach of fresnel equation.
  //      clamp(fresnelBias + fresnelScale * pow(1 + dot(I, N), fresnelPower), 0.0, 1.0); (1)
  //
  vec3 normal = normalize(fs_in.normal);

  vec3 I = normalize(fs_in.position - fs_in.viewPosition.xyz);
  vec3 R = reflect(I, normal);
  I = normalize(I);

  vec3 redTextureCoordinate = refract(I, normal, Eta.r);
  vec3 greenTextureCoordinate = refract(I, normal, Eta.g);
  vec3 blueTextureCoordinate = refract(I, normal, Eta.b);
  float reflectionFactor = clamp(fresnelBias + fresnelScale * pow(1 + dot(I, normal), fresnelPower), 0.0, 1.0);

  vec4 reflectColor = texture(skybox, R);

  float colorRed = texture(skybox, redTextureCoordinate).r;
  float colorGreen = texture(skybox, greenTextureCoordinate).g;
  float colorBlue = texture(skybox, blueTextureCoordinate).b;
  vec4 refractColor = vec4(colorRed, colorGreen, colorBlue, 1.0);

  FragColor = mix(refractColor, reflectColor, reflectionFactor);
}
