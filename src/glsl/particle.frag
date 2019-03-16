#version 330 core

uniform sampler2D tex1;
uniform int u_enableTexture;

in vec2 f_texcoord;
in vec4 f_color;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main (void) {
	vec4 color = f_color * (u_enableTexture > 0 ? texture2D(tex1, f_texcoord) : vec4(1, 1, 1, 1));

    vec3 result = color.rgb;
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = color;
}
