#version 330 core

uniform sampler2D u_texture;
in vec2 v_texCoords;

void main() {
	gl_FragColor = texture2D(u_texture, v_texCoords);
}