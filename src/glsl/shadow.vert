#version 330 core

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;

out vec2 v_texCoords;

void main() {
	v_texCoords = a_texCoord;
	gl_Position = a_position;
}