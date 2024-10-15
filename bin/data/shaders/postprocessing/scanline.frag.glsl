/*
    final.frag.glsl
*/

#version 400


uniform sampler2D tex0;
uniform float width;
uniform float height;

in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    vec4 color = texture(tex0, v_TexCoords);

    int w = int(v_TexCoords.x * width);
    int pos = int(v_TexCoords.y * height); // Scaling to screen size
    
    if((pos % 2) == 0) {
        discard;
    } else {
        /*
        if((w % 2) == 0) {
            discard;
        } else {
            out_Color = color;
        }
        */
        out_Color = color;
    }   

    //out_Color = texture(tex0, v_TexCoords);
}