#version 410

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec3 fragPosition;
out vec3 fragNormal;

void main()
{
	gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);

	fragPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
        fragNormal = vertexNormal;
}
