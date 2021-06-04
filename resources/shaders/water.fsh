#version 410

in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 dirToLight;
uniform vec3 eyePosition;

out vec4 finalFragColor;

void main()
{
    vec3 color = vec3(0.0, 0.0, 1.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 r = reflect(-dirToLight, fragNormal);
    vec3 e = normalize(eyePosition - fragPosition);
    float spec = max(dot(r, e), 0.0);
    spec = pow(spec, 32.0);
    vec3 specular = lightColor * spec;

    float opacity = 0.5;
    finalFragColor = vec4(color + specular, opacity);
}
