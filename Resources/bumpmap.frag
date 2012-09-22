// texturing
#version 330 core
in vec3 Normal, LightDir, EyeV;
in vec2 TexCoord;
out vec4 out_FragColor;
in vec3 Tangent, Binormal;

struct SLightData
{
	vec4 Ambient, Diffuse, Specular;
};

uniform SLightData Light0;
uniform SLightData FrontMaterial;

uniform sampler2D Texture;
uniform sampler2D Bumpmap;

void main (void)  
{   
	vec4 Bump = texture2D(Bumpmap, TexCoord);  
	vec4 Albedo = texture2D(Texture, TexCoord);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    Bump = (Bump * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    vec3 BumpNormal = Normal + Bump.x * Tangent + Bump.y * Binormal;
	
    // Normalize the resulting bump normal.
    BumpNormal = normalize(BumpNormal);

    // Invert the light direction for calculations.
    vec3 MinusLightDir = -LightDir;

    // Calculate the amount of light on this pixel based on the bump map normal value.
    float LightIntensity = clamp(dot(BumpNormal, MinusLightDir), 0.0, 1.0);

    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    vec4 color = clamp(Light0.Diffuse * LightIntensity, 0.0, 1.0);

    // Combine the final bump light color with the texture color.
    color = color * Albedo;
	
	//out_FragColor = Albedo * Light;
	out_FragColor = color;
	//out_FragColor = vec4(TexCoord,0.0,1.0);
}      


