#version 330 core

uniform sampler2D u_texture;

uniform int isDiffuse;

in vec2 v_texCoords0;
in vec2 v_texCoords1;
in vec2 v_texCoords2;
in vec2 v_texCoords3;
in vec2 v_texCoords4;

const float center = 0.2270270270;
const float close  = 0.3162162162;
const float far    = 0.0702702703;

void main() {
    if(isDiffuse != 0) {
        gl_FragColor.rgb =  far    * texture2D(u_texture, v_texCoords0).rgb + 
                            close  * texture2D(u_texture, v_texCoords1).rgb + 
                            center * texture2D(u_texture, v_texCoords2).rgb + 
                            close  * texture2D(u_texture, v_texCoords3).rgb +
                            far    * texture2D(u_texture, v_texCoords4).rgb;
    }
    else {
        gl_FragColor =  far    * texture2D(u_texture, v_texCoords0) + 
                        close  * texture2D(u_texture, v_texCoords1) + 
                        center * texture2D(u_texture, v_texCoords2) + 
                        close  * texture2D(u_texture, v_texCoords3) +
                        far    * texture2D(u_texture, v_texCoords4);
    }
    
}