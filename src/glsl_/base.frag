#version 330 core

uniform sampler2D tex;
uniform vec4 u_color;

in vec2 f_texcoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

out vec4 o_color;

void main (void) {
    vec3 result = u_color.rgb;
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = texture2D(tex, f_texcoord) * u_color;;
}
