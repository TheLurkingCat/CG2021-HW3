#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out float height;

uniform float offset;

void main() {
    const float delta = 0.01;
    // TODO: Generate the normal map.
    //   1. Get the position of the fragment. (screen space)
    //   2. Sample 4 points from combination of x +- delta, y +- delta
    //   3. Form at least 2 triangles from those points. Calculate their surface normal
    //   4. Average the surface normal, then tranform the normal [-1, 1] to RGB [0, 1]
    //   5. (Bonus) Output the actual height to different location. e.g. layout(location = 1)
    // Note:
    //   1. Height at (x, y) = H(x, y) = sin(offset - 0.25 * y)
    //   2. A simple tranform from [-1, 1] to [0, 1] is f(x) = x * 0.5 + 0.5
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;

    vec3 p1 = vec3(x + delta, y + delta, sin(offset - 0.25 * (y + delta)));
    vec3 p2 = vec3(x - delta, y + delta, sin(offset - 0.25 * (y + delta)));
    vec3 p3 = vec3(x - delta, y - delta, sin(offset - 0.25 * (y - delta)));
    vec3 p4 = vec3(x + delta, y - delta, sin(offset - 0.25 * (y - delta)));

    vec3 n1 = normalize(cross(p2 - p1, p3 - p1));
    vec3 n2 = normalize(cross(p3 - p1, p4 - p1));
    vec3 n = normalize(n1 + n2) * 0.5 + 0.5;
    FragColor = vec4(n, 1.0);
    height = sin(offset - 0.15 * y);
}
