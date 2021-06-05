#version 410 core

// Number of CPs in the output patch
layout (vertices = 4) out;

uniform vec3 eyeWorldPos;

// Input CPs attributes
in vec3 worldPos_TCS_in[];
in vec3 normal_TCS_in[];

// Output CPs attributes
out vec3 worldPos_TES_in[];
out vec3 normal_TES_in[];

void main()
{
    worldPos_TES_in[gl_InvocationID] = worldPos_TCS_in[gl_InvocationID];
    normal_TES_in[gl_InvocationID] = normal_TCS_in[gl_InvocationID];

    // Set the tessellation level of the outer ring
    // Since our domain is a triangle, we only access the first three for the outer level
    gl_TessLevelOuter[0] = 64.0;
    gl_TessLevelOuter[1] = 64.0;
    gl_TessLevelOuter[2] = 64.0;
    gl_TessLevelOuter[3] = 64.0;

    // Set the tessellation level of the inner geometry, i.e., number of rings
    gl_TessLevelInner[0] = 64.0;
    gl_TessLevelInner[1] = 64.0;
}

