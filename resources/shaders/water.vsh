#version 410

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 modelMatrix;

out vec3 worldPos_TCS_in;
out vec3 normal_TCS_in;

void main()
{
    worldPos_TCS_in = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    normal_TCS_in = vertexNormal;
}
