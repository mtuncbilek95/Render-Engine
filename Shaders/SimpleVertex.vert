#version 460

vec2 positions[4] = vec2[](
    vec2(1, -1),
    vec2(1, 1),
    vec2(-1, 1),
    vec2(-1, -1)
);

int indices[6] = int[](
    0, 1, 2,
    0, 2, 3
);

void main() {
    int index = indices[gl_VertexIndex];
    gl_Position = vec4(positions[index], 0.0, 1.0);
}