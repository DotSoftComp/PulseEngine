#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out; // 6 faces × 3 vertices

in vec4 FragPos[];

uniform mat4 shadowMatrices[6]; // one for each face

out vec4 FragPosWorld; // pass to fragment shader

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // choose cubemap face

        for (int i = 0; i < 3; ++i)
        {
            FragPosWorld = FragPos[i];
            gl_Position = shadowMatrices[face] * FragPos[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}