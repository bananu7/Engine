// texturing
#version 330 core
in vec2 TexCoord;
flat in float Height;
in vec3 Normal, LightDir, EyeV;//, TexMap;

out vec4 out_FragColor;

struct SLightData
{
	vec4 Ambient, Diffuse, Specular;
};

uniform SLightData Light0;
uniform SLightData FrontMaterial;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Bumpmap;

void main (void)  
{   
	vec4 LightColor = 
	//(/*gl_FrontLightModelProduct.sceneColor*/ 1.0 * FrontMaterial.Ambient) + 
	(Light0.Ambient * FrontMaterial.Ambient);

	vec3 N = normalize(Normal);
	vec3 L = normalize(LightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		LightColor += Light0.Diffuse * 
		              FrontMaterial.Diffuse *
					  lambertTerm;

		vec3 E = normalize(EyeV);
		vec3 R = reflect(-L, N);
		//float specular = 1;
		//float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
		float specular = pow( max(dot(R, E), 0.0), 20.0 );

		LightColor += Light0.Specular *
		              FrontMaterial.Specular *
					  specular;
	}
	
	//float T = normalize(TexMap);

	vec4 TextureColor = texture2D(Texture1, TexCoord);// * T;
	vec4 Bump  = texture2D(Bumpmap, TexCoord);
	TextureColor.a = 1.0;
	out_FragColor = LightColor * TextureColor;
	//ut_FragColor = Light0.Diffuse;
	//out_FragColor = vec4(Normal, 1.0);
	//out_FragColor = vec4(TexCoord/12.7,0.0,1.0); // czyli texcoordy sa juz dobrze
	//out_FragColor = vec4(TexCoord.s,TexCoord.t,TexCoord.r,1.0);
	//out_FragColor = vec4(0.0, Height/10, 0.0, 1.0);
}      


