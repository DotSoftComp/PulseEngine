#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;

out vec4 FragPosWorld; // pass world-space position to GS

void main()
{
    FragPosWorld = model * vec4(aPos, 1.0);
}
