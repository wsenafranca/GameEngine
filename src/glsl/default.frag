#version 330 core

uniform sampler2D tex;
uniform int u_enableTexture = 1;
uniform int u_blur = 0;

in vec2 f_texcoord;
in vec4 f_color;
in vec2 f_position;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BlurColor;

void main (void) {
	vec4 color = (u_enableTexture != 0 ? f_color * texture2D(tex, f_texcoord) : f_color);
	FragColor = color;
	BlurColor = u_blur != 0 ? color : vec4(0, 0, 0, 0);
}
