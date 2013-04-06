// lighting
#version 330 core
uniform mat4 ViewMatrix, ProjectionMatrix, ModelMatrix;
uniform vec3 LightPosition;

in layout(location=0)vec4 in_Position;
in layout(location=1)vec3 in_Normal;
in layout(location=2)vec2 in_TexCoord;
//in layout(location=3)vec2 in_TexMap;

out vec2 TexCoord;
flat out float Height;
out vec3 Normal;
out vec3 LightDir;
out vec3 EyeV;
//out vec3 TexMap;

void main(void)
{
    mat3 NormalMatrix = transpose(inverse(mat3(ModelMatrix)));

    gl_Position = ProjectionMatrix * ViewMatrix * in_Position * ModelMatrix;

    TexCoord = in_TexCoord;
    Height = in_Position.y;
    vec3 MVert = (ViewMatrix * in_Position).xyz;
    Normal = NormalMatrix * in_Normal;
    LightDir = vec3(LightPosition - MVert);
    EyeV = -MVert;
    //TexMap = in_TexMap;
}