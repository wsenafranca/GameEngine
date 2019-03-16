#version 330 core

layout (location = 0) in vec4 a_vertex;

out vec2 f_texcoord;

void main(void)
{
	gl_Position = vec4(a_vertex.xy, 0, 1);
	f_texcoord = a_vertex.zw;
}
