#version 330 core

uniform int isGammaCorrection;

in vec4 v_color;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BlurColor;

void main() {
	BlurColor = (isGammaCorrection != 0 ? sqrt(v_color) : v_color);
}