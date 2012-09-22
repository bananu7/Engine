// simple
#version 330 core
uniform mat4 ViewMatrix, ProjectionMatrix, ModelMatrix;

in layout(location=0)vec4 in_Position;
in layout(location=1)vec3 in_Normal;
in layout(location=2)vec2 in_TexCoord;

void main(void)
{
	gl_Position = ProjectionMatrix *
				  ViewMatrix *
				  ModelMatrix *
				  in_Position;
}