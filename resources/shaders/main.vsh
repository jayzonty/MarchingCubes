#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 mvpMatrix;

out vec4 fragColor;
out vec2 fragUV;
out vec3 fragNormal;

void main()
{
	gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);

	fragColor = vertexColor;
	fragUV = vertexUV;
	fragNormal = vertexNormal;
}

