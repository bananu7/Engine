// lighting
#version 330 core
in vec3 Normal, LightDir, EyeV;
in vec2 TexCoord;
out vec4 out_FragColor;

struct SLightData
{
	vec4 Ambient, Diffuse, Specular;
};

uniform SLightData Light0;
uniform SLightData FrontMaterial;

uniform sampler2D in_Texture;

void main (void)  
{     
   vec4 final_color = 
	//(/*gl_FrontLightModelProduct.sceneColor*/ 1.0 * FrontMaterial.Ambient) + 
	(Light0.Ambient * FrontMaterial.Ambient);

	vec3 N = normalize(Normal);
	vec3 L = normalize(LightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += Light0.Diffuse * 
		               FrontMaterial.Diffuse *
					   lambertTerm;

		vec3 E = normalize(EyeV);
		vec3 R = reflect(-L, N);
		//float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
		float specular = pow( max(dot(R, E), 0.0), 20.0 );

		final_color += Light0.Specular *
		               FrontMaterial.Specular *
					   specular;
	}			   

	//final_color = vec4(0.3, 0.2, 1.0, 1.0);
	//final_color = vec4(TexCoord, 0.0, 1.0);
	out_FragColor = vec4(final_color.rgb, 1.0);
}      