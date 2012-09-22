// simple
#version 330 core
uniform mat4 ViewMatrix, ProjectionMatrix, ModelMatrix;

in layout(location=0)vec3 in_Position;
in layout(location=1)vec2 in_TexCoord;

out vec2 TexCoord;

void main(void)
{

	vec3 Position = in_Position;

	//Position = reflect(Position, vec3(0, 1, 0));

	vec4 Position4 = vec4(Position, 1);
	/*Position.z = 0;
	Position.w = 1; */
	gl_Position = ProjectionMatrix *
				  ViewMatrix *
				  ModelMatrix *
				  Position4;
	TexCoord = in_TexCoord;
}