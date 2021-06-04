#version 410

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 mvpMatrix;

out vec4 fragColor;

void main()
{
	gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
	fragColor = vertexColor;
}

