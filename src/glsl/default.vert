#version 330 core

uniform mat4 u_Projection;
uniform mat4 u_View;

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_texcoord;

out vec2 f_texcoord;
out vec4 f_color;

void main(void)
{
	vec4 v = u_Projection*u_View*vec4(a_position, 0, 1);
	gl_Position = v;
	f_texcoord = a_texcoord;
	f_color = a_color;
	f_color.a = f_color.a*(255.0/254.0);
}
