#version 330 core

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;

uniform vec2 dir;
uniform vec2 fboSize;

out vec2 v_texCoords0;
out vec2 v_texCoords1;
out vec2 v_texCoords2;
out vec2 v_texCoords3;
out vec2 v_texCoords4;

void main() {
	vec2 futher = vec2(3.2307692308 / fboSize.x, 3.2307692308 / fboSize.y );
	vec2 closer = vec2(1.3846153846 / fboSize.x, 1.3846153846 / fboSize.y );

	vec2 f = futher * dir;
	vec2 c = closer * dir;
	v_texCoords0 = a_texCoord - f;
	v_texCoords1 = a_texCoord - c;
	v_texCoords2 = a_texCoord;
	v_texCoords3 = a_texCoord + c;
	v_texCoords4 = a_texCoord + f;
	gl_Position = a_position;
}
