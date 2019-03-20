#version 330 core

uniform mat4 u_MVP;

layout (location = 0) in vec2 a_vertex;

out vec2 f_texcoord;

void main(void)
{
	gl_Position = u_MVP * vec4(a_vertex, 0, 1);
	f_texcoord = vec2(0.5 + a_vertex.x, a_vertex.y+0.5f);
}
