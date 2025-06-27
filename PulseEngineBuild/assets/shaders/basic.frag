#version 330 core
#define MAX_LIGHTS 10

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform float lightIntensities[MAX_LIGHTS];
uniform float lightAttenuations[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 objectColor;
uniform float specularStrength;

void main()
{
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < lightCount; ++i)
    {
        // Point Light Direction
        vec3 lightDir = normalize(lightPositions[i] - FragPos);

        // Ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColors[i];

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColors[i];

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColors[i];

        // Attenuation
        float distance = length(lightPositions[i] - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

        // Accumulate lighting
        vec3 lighting = (ambient + diffuse + specular) * attenuation * lightIntensities[i];
        result += lighting * objectColor;
    }

    FragColor = vec4(result, 1.0);
}
