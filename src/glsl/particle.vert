#version 330 core

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

layout (location = 0) in vec2 a_vertex;
layout (location = 1) in vec4 a_transforms;
layout (location = 2) in vec4 a_color;

out vec2 f_texcoord;
out vec4 f_color;

mat4 rotate(float theta) {
	float c = cos(theta);
	float s = sin(theta);
	float t = 1-c;

	return mat4(vec4(   c,   -s, 0.0f, 0.0f), 
				vec4(   s,    c, 0.0f, 0.0f), 
				vec4(0.0f, 0.0f,  t+c, 0.0f), 
				vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

mat4 model(vec4 t) {
	mat4 mt = mat4(	vec4(1.0f, 0.0f, 0.0f,  t.x), 
					vec4(0.0f, 1.0f, 0.0f,  t.y), 
					vec4(0.0f, 0.0f, 1.0f, 0.0f), 
					vec4(0.0f, 0.0f, 0.0f, 1.0f));

	mat4 ms = mat4(	vec4( t.z, 0.0f, 0.0f, 0.0f), 
					vec4(0.0f,  t.z, 0.0f, 0.0f), 
					vec4(0.0f, 0.0f, 1.0f, 0.0f), 
					vec4(0.0f, 0.0f, 0.0f, 1.0f));

	return rotate(t.w) * ms * mt;
}

void main(void)
{
	vec4 v1 = vec4(a_vertex, 0.0f, 1.0f) * model(a_transforms);
	vec4 v2 = u_Projection * u_View * u_Model * v1;
	gl_Position = v2;
	f_texcoord = vec2(0.5 + a_vertex.x, 0.5f-a_vertex.y);
	f_color = a_color;
}
