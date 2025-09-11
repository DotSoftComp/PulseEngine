#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

// Material
uniform vec3 viewPos;
uniform vec3 objectColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;

// PBR parameters
uniform sampler2D roughnessMap;    // 0.0 = smooth, 1.0 = rough
float roughness;

uniform float metallic;     // 0.0 = dielectric, 1.0 = metal

// Light structs
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float attenuation;
    bool castsShadow;
    float farPlane;
    samplerCube depthMap;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
    bool castsShadow;
    sampler2D shadowMap;
    mat4 lightSpaceMatrix;
};

uniform PointLight pointLights[4];
uniform int numPointLights;
uniform DirectionalLight dirLight;

// -------------------- Shadow Calculations --------------------

float CalculateDirectionalShadow(DirectionalLight light, vec3 fragPos, vec3 normal)
{
    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(light.shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(normal, -light.direction)), 0.005);

    return (currentDepth  > closestDepth) ? 1.0 : 0.0;
}

float CalculatePointShadow(PointLight light, vec3 fragPos, vec3 normal)
{
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);
    float closestDepth = texture(light.depthMap, normalize(fragToLight)).r * light.farPlane;

    float bias = max(0.05 * (1.0 - dot(normal, normalize(-fragToLight))), 0.005);
    if (currentDepth - bias > closestDepth)
        return 1.0;
    return 0.0;
}

// -------------------- Normal Mapping --------------------

vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(FragPos);
    vec3 Q2 = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1 * st2.y - Q2 * st1.y); // use .y
    vec3 B = normalize(cross(N, T));             // remove minus for conventional tangent space

    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}
// -------------------- PBR Helpers --------------------

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265 * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// -------------------- Light Computations --------------------

vec3 ComputePointLight(PointLight light, vec3 N, vec3 V, vec3 albedo)
{
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L);
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0 + light.attenuation * (dist * dist));

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);

    float shadow = light.castsShadow ? CalculatePointShadow(light, FragPos, N) : 0.0;

    vec3 radiance = light.color * light.intensity * attenuation;
    return (1.0 - 0.0f) * ((kD * albedo / 3.14159265 + specular) * radiance * NdotL);
}

vec3 ComputeDirectionalLight(DirectionalLight light, vec3 N, vec3 V, vec3 albedo)
{
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);

    float shadow = light.castsShadow ? CalculateDirectionalShadow(light, FragPos, N) : 0.0;

    vec3 radiance = light.color * light.intensity;
    return (1.0 - 0.0f) * ((kD * albedo / 3.14159265 + specular) * radiance * NdotL);
}

// -------------------- Main --------------------

void main()
{
    vec3 albedoColor = texture(albedoMap, TexCoords).rgb;
    roughness = texture(roughnessMap, TexCoords).r;
        
    albedoColor *= objectColor;

    vec3 N = GetNormalFromMap();
    vec3 V = normalize(viewPos - FragPos);

    vec3 lightingResult = vec3(0.0);

    for (int i = 0; i < numPointLights; ++i)
        lightingResult += ComputePointLight(pointLights[i], N, V, albedoColor);

    lightingResult += ComputeDirectionalLight(dirLight, N, V, albedoColor);

    FragColor = vec4(lightingResult, 1.0);
}
