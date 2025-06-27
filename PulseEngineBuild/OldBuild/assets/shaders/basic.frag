#version 330 core
#define MAX_LIGHTS 10
#define MAX_SHADOWS 10

in vec3 FragPos;  
in vec3 Normal;  

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform float lightIntensities[MAX_LIGHTS];
uniform float lightAttenuations[MAX_LIGHTS];
uniform int lightTypes[MAX_LIGHTS]; // 0 = directional, 1 = point
uniform vec3 objectColor;
uniform float specularStrength;
uniform int lightCount;

uniform mat4 lightSpaceMatrices[MAX_SHADOWS];
uniform sampler2D shadowMaps[MAX_SHADOWS];
uniform int shadowCasterCount;
uniform int shadowCasterIndices[MAX_SHADOWS];

float CalculateShadow(int casterIndex, vec3 fragPos)
{
    vec4 fragPosLightSpace = lightSpaceMatrices[casterIndex] * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    float shadow = 0.0;
    float bias = max(0.05 * (1.0 - dot(Normal, normalize(lightPositions[shadowCasterIndices[casterIndex]] - fragPos))), 0.005);

    vec2 texelSize = 1.0 / textureSize(shadowMaps[casterIndex], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMaps[casterIndex], projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}



void main()
{
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < lightCount; i++) {
        // Ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColors[i];

        // Diffuse
        vec3 lightDir = normalize(lightPositions[i] - FragPos);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColors[i];

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColors[i];

        // Attenuation
        float distance = length(lightPositions[i] - FragPos);
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
        diffuse *= attenuation;
        specular *= attenuation;

        // Shadow (only for shadow caster)
        float shadow = 0.0;
        for (int j = 0; j < shadowCasterCount; ++j)
        {
            if (i == shadowCasterIndices[j])
            {
                if(lightTypes[i] == 0) shadow = CalculateShadow(j, FragPos);
                break; // only one shadow map per light
            }
        }

        // Accumulate result
        vec3 lighting = ambient + ((1.0 - shadow) * (diffuse + specular));
        result += lighting * objectColor;
    }

    FragColor = vec4(result, 1.0);
}
