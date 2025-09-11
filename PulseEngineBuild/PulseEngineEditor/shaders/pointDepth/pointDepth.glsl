#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out; // 6 faces × 3 vertices

uniform mat4 shadowMatrices[6];

in vec4 FragPosWorld[];
out vec4 GS_FragPosWorld;

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // select cube face
        for (int i = 0; i < 3; ++i)
        {
            GS_FragPosWorld = FragPosWorld[i];
            gl_Position = shadowMatrices[face] * FragPosWorld[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}
