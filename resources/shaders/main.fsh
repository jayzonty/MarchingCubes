#version 330

in vec3 fragPosition;
in vec4 fragColor;
in vec2 fragUV;
in vec3 fragNormal;

uniform vec3 lightDir;

out vec4 finalFragColor;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    float col1 = max(dot(fragNormal, vec3(1.0, 0.0, 0.0)), 0.0);
    float col2 = max(dot(fragNormal, vec3(0.0, 1.0, 0.0)), 0.0);
    float col3 = max(dot(fragNormal, vec3(0.0, 0.0, 1.0)), 0.0);
    vec3 color = vec3(col1, col2, col3);

    vec3 dirToLight = normalize(-lightDir);

    float ambient = 0.25f;
    vec3 ambientColor = lightColor * color * ambient;

    float diffuse = max(dot(dirToLight, fragNormal), 0.0);
    vec3 diffuseColor = lightColor * color * diffuse;

    color = ambientColor + diffuseColor;

    finalFragColor = vec4(color, fragColor.a);
}

