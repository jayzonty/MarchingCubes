#version 330

in vec3 fragPosition;
in vec4 fragColor;
in vec2 fragUV;
in vec3 fragNormal;

uniform sampler2D tex;

uniform vec3 lightPos;

out vec4 finalFragColor;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 color = vec3(fragColor);

    vec3 dirToLight = normalize(lightPos - fragPosition);

    float ambient = 0.15f;
    vec3 ambientColor = lightColor * ambient;

    float diffuse = max(dot(dirToLight, fragNormal), 0.0);
    vec3 diffuseColor = lightColor * diffuse;

    color = color * (ambientColor + diffuseColor);

    finalFragColor = vec4(color, fragColor.a);
}

