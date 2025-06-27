#version 330 core

layout(location = 0) in vec3 position;  // This is the input vertex position
layout(location = 1) in vec3 normal;    // If you use normal data

uniform mat4 lightSpaceMatrix;  // Light's space transformation matrix
uniform mat4 model;             // Model transformation matrix

void main() {
    // The final vertex position in light space
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0);
}
