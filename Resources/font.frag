#version 330

precision highp float;
uniform vec4 Color;
uniform sampler2D Texture;

in vec2 TexCoord; 
out vec4 out_FragColor;

void main(void)
{
	vec4 FragColor;
	FragColor = texture2D(Texture, TexCoord);
	 
	float AlphaTest = 1.0;
	AlphaTest = min(AlphaTest, FragColor.r);
	AlphaTest = min(AlphaTest, FragColor.g);
	AlphaTest = min(AlphaTest, FragColor.b);
	
	FragColor *= Color;
	FragColor.a = AlphaTest * 2;
	out_FragColor = FragColor;
}