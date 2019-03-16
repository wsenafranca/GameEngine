#version 330 core

uniform mat4 u_model, u_view, u_projection;

layout (location = 0) in vec4 a_vertex;

out vec2 f_texcoord;

void main(void)
{
	gl_Position = (u_projection * u_view * u_model) * vec4(a_vertex.xy, 0, 1);
	f_texcoord = a_vertex.zw;
}
