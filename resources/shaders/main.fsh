#version 330

in vec4 fragColor;
in vec2 fragUV;
in vec3 fragNormal;

uniform sampler2D tex;

out vec4 finalFragColor;

void main()
{
    finalFragColor = fragColor;
}

