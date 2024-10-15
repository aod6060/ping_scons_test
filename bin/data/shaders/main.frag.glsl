/*
    main.frag.glsl
*/
#version 400

in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    //out_Color = vec4(v_TexCoords, 0.0, 1.0);
    out_Color = vec4(1.0);
}