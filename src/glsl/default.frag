#version 330 core

uniform sampler2D tex;
uniform int u_enableTexture = 1;

in vec2 f_texcoord;
in vec4 f_color;

out vec4 FragColor;

void main (void) {
	FragColor = f_color * (u_enableTexture != 0 ? texture2D(tex, f_texcoord) : vec4(1, 1, 1, 1));
}
