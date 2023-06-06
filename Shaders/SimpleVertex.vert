#version 460

#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) out vec3 fragColor;

vec2 pos[3] = vec2[](
        vec2( 0.0,-0.5),
        vec2( 0.5, 0.5),
        vec2(-0.5, 0.5)
);

vec3 colorPos[3] = vec3[](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(pos[gl_VertexIndex].x, 0.0, 1.0);
    fragColor = colorPos[gl_VertexIndex];
}