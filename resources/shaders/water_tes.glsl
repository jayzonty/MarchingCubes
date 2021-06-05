#version 410 core

// quads - What domain are we working on
// equal_spacing - Each segment will be divided equally
// ccw - Primitive generator will output triangles in CCW order
layout (quads, equal_spacing, ccw) in;

uniform mat4 vpMatrix;
uniform float time;

in vec3 worldPos_TES_in[];
in vec3 normal_TES_in[];

out vec3 worldPos_FS_in;
out vec3 normal_FS_in;

vec3 interpolateQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
{
    vec3 a = mix(v0, v1, gl_TessCoord.x);
    vec3 b = mix(v3, v2, gl_TessCoord.x);
    return mix(a, b, gl_TessCoord.y);
}

void main()
{
    worldPos_FS_in = interpolateQuad(worldPos_TES_in[0], worldPos_TES_in[1], worldPos_TES_in[2], worldPos_TES_in[3]);
    worldPos_FS_in += vec3(0.0, 1.0, 0.0) * sin(worldPos_FS_in.x * time);

    normal_FS_in = interpolateQuad(normal_TES_in[0], normal_TES_in[1], normal_TES_in[2], normal_TES_in[3]);
    normal_FS_in = normalize(normal_FS_in);

    gl_Position = vpMatrix * vec4(worldPos_FS_in, 1.0);
}

