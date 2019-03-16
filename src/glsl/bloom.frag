#version 330 core
out vec4 FragColor;

in vec2 f_texcoord;

uniform sampler2D u_scene;
uniform sampler2D u_bloomBlur;
uniform bool u_bloom;
uniform float u_exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_scene, f_texcoord).rgb;      
    vec3 bloomColor = texture(u_bloomBlur, f_texcoord).rgb;
    if(u_bloom)
        hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * u_exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}