#version 330 core

uniform mat4 u_projTrans;
uniform mat4 u_View;

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_texcoord;

out vec2 f_texcoord;
out vec4 f_color;
out vec2 f_position;

void main(void)
{
	gl_Position = u_projTrans*vec4(a_position, 0, 1);
	f_texcoord = a_texcoord;
	f_color.rgb = a_color.rgb;
	f_color.a = a_color.a*(255.0/254.0);
	f_position = a_position;
}
