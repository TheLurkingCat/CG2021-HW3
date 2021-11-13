#version 330 core
layout(location = 0) out vec4 FragColor;

uniform vec2 center;
uniform float offset;

void main() {
    const float delta = 0.5;
    vec2 position = gl_FragCoord.xy - center;
    float x = position.x;
    float y = position.y;

    vec3 p1 = vec3(x + delta, y + delta, sin(offset - 0.15 * (y + delta)));
    vec3 p2 = vec3(x - delta, y + delta, sin(offset - 0.15 * (y + delta)));
    vec3 p3 = vec3(x - delta, y - delta, sin(offset - 0.15 * (y - delta)));
    vec3 p4 = vec3(x + delta, y - delta, sin(offset - 0.15 * (y - delta)));

    vec3 n1 = normalize(cross(p2 - p1, p3 - p1));
    vec3 n2 = normalize(cross(p3 - p1, p4 - p1));
    vec3 n = normalize(n1 + n2) * 0.5 + 0.5;
    FragColor = vec4(n, 1.0);
}
