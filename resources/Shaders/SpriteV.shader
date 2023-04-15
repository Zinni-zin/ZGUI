#version 450 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_MVP;

out vec4 v_Colour;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	v_Colour = a_Colour;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = u_MVP * a_Position;
};
