#version 330
 
//in  vec3 ex_Color;
layout(location = 0) out vec4 out_FragColor;

in vec2 TexCoord;

uniform sampler2D Screen;
 
void main(void)
{
	vec4 Color = texture(Screen, TexCoord);
	Color.r *= 0.8;
	Color.b *= 1.2;
	Color.b += 0.1;
	Color.a = 1;
	out_FragColor = Color;//vec4(TexCoord, 0.0, 1.0);
	// vec4(ex_Color,1.0);
}