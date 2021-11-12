#version 330 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
  vec3 position;
  vec3 normal;
  flat vec3 viewPosition;
} fs_in;

uniform samplerCube skybox;

void main() {
  float fresnelBias = 0.5;
  float fresnelScale = 0.4;
  float fresnelPower = 2;

  vec3 etaRatio = vec3(1/ 1.39, 1 / 1.44, 1 / 1.47);
  vec3 normal = normalize(fs_in.normal);

  vec3 I = normalize(fs_in.position - fs_in.viewPosition.xyz);
  vec3 R = reflect(I, normal);
  I = normalize(I);

  vec3 redTextureCoordinate = refract(I, normal, etaRatio.r);
  vec3 greenTextureCoordinate = refract(I, normal, etaRatio.g);
  vec3 blueTextureCoordinate = refract(I, normal, etaRatio.b);
  float reflectionFactor = clamp(fresnelBias + fresnelScale * pow(1 + dot(I, normal), fresnelPower), 0.0, 1.0);

  vec4 reflectColor = texture(skybox, R);

  float colorRed = texture(skybox, redTextureCoordinate).r;
  float colorGreen = texture(skybox, greenTextureCoordinate).g;
  float colorBlue = texture(skybox, blueTextureCoordinate).b;
  vec4 refractColor = vec4(colorRed, colorGreen, colorBlue, 1.0);

  FragColor = mix(refractColor, reflectColor, reflectionFactor);
}
