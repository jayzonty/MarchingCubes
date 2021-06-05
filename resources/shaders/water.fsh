#version 410

in vec3 worldPos_FS_in;
in vec3 normal_FS_in;

uniform vec3 dirToLight;
uniform vec3 eyeWorldPos;

out vec4 finalFragColor;

void main()
{
    vec3 color = vec3(0.0, 0.0, 1.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 r = reflect(-dirToLight, normal_FS_in);
    vec3 e = normalize(eyeWorldPos - worldPos_FS_in);
    float spec = max(dot(r, e), 0.0);
    spec = pow(spec, 32.0);
    vec3 specular = lightColor * spec;

    float opacity = 0.5;
    finalFragColor = vec4(color + specular, opacity);
}
