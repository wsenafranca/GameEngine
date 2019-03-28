#version 330 core

layout (location = 0) in vec4 vertex_positions;
layout (location = 1) in vec4 quad_colors;
layout (location = 2) in float s;

uniform mat4 u_projTrans;

out vec4 v_color;

void main() {
	v_color = s * quad_colors;
	gl_Position =  u_projTrans * vertex_positions;
}