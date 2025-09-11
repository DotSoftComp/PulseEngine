#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Material
uniform vec3 viewPos;
uniform vec3 objectColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightingResult = vec3(0.0);

    vec4 albedoColor = texture(albedoMap, TexCoords);

    // Heuristic: consider albedo invalid if color is close to black or alpha near zero

    vec3 baseColor = albedoColor.rgb;

    FragColor = vec4(baseColor, 1.0);
}
 