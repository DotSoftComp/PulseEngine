#version 460 core
in vec4 GS_FragPosWorld;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    float lightDist = length(GS_FragPosWorld.xyz - lightPos);
    // map distance to [0,1] depth
    gl_FragDepth = lightDist / farPlane;
}
