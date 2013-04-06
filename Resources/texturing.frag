// texturing
#version 330 core
in vec2 TexCoord;
out vec4 out_FragColor;

uniform float Mood;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Bumpmap;

void main (void)  
{     
    vec4 Tex0 = texture2D(Texture0, TexCoord);
    vec4 Tex1 = texture2D(Texture1, TexCoord);
    if(Tex1 == vec4(0.0)) Tex1 = vec4(1.0,1.0,1.0,1.0);
    //vec4 Bump = texture2D(Bumpmap, TexCoord);
    //out_FragColor = Color * Bump;
    out_FragColor = Tex0 * Mood + Tex1 * (1 - Mood);
    //out_FragColor.a = 1.0;
}      


