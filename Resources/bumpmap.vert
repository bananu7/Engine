// bumpmapping
#version 330 core
uniform mat4 ViewMatrix, ProjectionMatrix;
uniform vec3 LightPosition;

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

out vec2 TexCoord;

out vec3 Normal;
out vec3 LightDir;
out vec3 EyeV;
out vec3 Tangent, Binormal;

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * in_Position;

	// Texturing
	TexCoord = in_TexCoord;

	// Lighting
	mat3 NormalMatrix = transpose(inverse(mat3(ViewMatrix)));
	vec3 MVert = (ViewMatrix * in_Position).xyz;
	Normal = NormalMatrix * in_Normal;
	LightDir = vec3(LightPosition - MVert);
	EyeV = -MVert;

	// Calculate the tangent vector against the world matrix only and then normalize the final value.
    Tangent = mul(input.tangent, mat3(worldMatrix));
    Tangent = normalize(Tangent);

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    Binormal = mul(input.binormal, mat3(worldMatrix));
    Binormal = normalize(output.binormal);

}