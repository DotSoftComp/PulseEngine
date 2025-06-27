uniform mat4 shadowMatrix;
uniform mat4 model;

void main()
{
    gl_Position = shadowMatrix * model * vec4(aPos, 1.0);
}
