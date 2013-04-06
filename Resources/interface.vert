#version 330 core
uniform mat4 ViewMatrix, ProjectionMatrix, ModelMatrix;

in layout(location=0)vec4 in_Position;
in layout(location=1)vec3 in_Normal;
in layout(location=2)vec2 in_TexCoord;

out vec4 Color;

void main(void)
{

    vec4 Position = in_Position;
    Position.z = 0;
    Position.w = 1; 
    gl_Position = ProjectionMatrix *
                  Position;
    Color = vec4 (in_Normal,1.f);
}