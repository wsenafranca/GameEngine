#version 330 core

uniform sampler2D u_texture;
uniform vec4 ambient;

in vec2 v_texCoords;

void main() {
	vec4 c = texture2D(u_texture, v_texCoords);
	gl_FragColor.rgb = c.rgb * c.a + ambient.rgb;
	gl_FragColor.a = ambient.a - c.a;
}