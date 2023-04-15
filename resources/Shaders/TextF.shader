#version 450 core

layout(location = 0) out vec4 o_Colour;

in vec4 v_Colour;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	o_Colour = texture(u_Textures[index], v_TexCoord).r * v_Colour;
};