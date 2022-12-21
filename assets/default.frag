#version 430 core
uniform vec4 vertColor;
in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

void main()
{
	frag_color = texture(myTexture, TexCoord);
};