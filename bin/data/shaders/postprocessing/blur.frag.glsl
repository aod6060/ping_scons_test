#version 400


uniform sampler2D tex0;

uniform float width;
uniform float height;
uniform float blurAmount;

in vec2 v_TexCoords;
out vec4 out_Color;


vec3 blurHorizontal(sampler2D s, vec2 tc, float blurWidth) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec2(tc.x - 4.0*blurWidth, tc.y)).rgb * 0.05;
    sum += texture(s, vec2(tc.x - 3.0*blurWidth, tc.y)).rgb * 0.09;
    sum += texture(s, vec2(tc.x - 2.0*blurWidth, tc.y)).rgb * 0.12;
    sum += texture(s, vec2(tc.x - blurWidth, tc.y)).rgb * 0.15;
    sum += texture(s, vec2(tc.x, tc.y)).rgb * 0.16;
    sum += texture(s, vec2(tc.x + blurWidth, tc.y)).rgb * 0.15;
    sum += texture(s, vec2(tc.x + 2.0*blurWidth, tc.y)).rgb * 0.12;
    sum += texture(s, vec2(tc.x + 3.0*blurWidth, tc.y)).rgb * 0.09;
    sum += texture(s, vec2(tc.x + 4.0*blurWidth, tc.y)).rgb * 0.05;
    return sum;
}

vec3 blurVerticle(sampler2D s, vec2 tc, float blurHeight) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec2(tc.x, tc.y - 4.0*blurHeight)).rgb * 0.05;
    sum += texture(s, vec2(tc.x, tc.y - 3.0*blurHeight)).rgb * 0.09;
    sum += texture(s, vec2(tc.x, tc.y - 2.0*blurHeight)).rgb * 0.12;
    sum += texture(s, vec2(tc.x, tc.y - blurHeight)).rgb * 0.15;
    sum += texture(s, vec2(tc.x, tc.y)).rgb * 0.16;
    sum += texture(s, vec2(tc.x, tc.y + blurHeight)).rgb * 0.15;
    sum += texture(s, vec2(tc.x, tc.y + 2.0*blurHeight)).rgb * 0.12;
    sum += texture(s, vec2(tc.x, tc.y + 3.0*blurHeight)).rgb * 0.09;
    sum += texture(s, vec2(tc.x, tc.y + 4.0*blurHeight)).rgb * 0.05;
    return sum;
}

vec3 blurDiagonal1(sampler2D s, vec2 tc, float blurWidth, float blurHeight) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec2(tc.x - 4.0*blurWidth, tc.y - 4.0*blurHeight)).rgb * 0.05;
    sum += texture(s, vec2(tc.x - 3.0*blurWidth, tc.y - 3.0*blurHeight)).rgb * 0.09;
    sum += texture(s, vec2(tc.x - 2.0*blurWidth, tc.y - 2.0*blurHeight)).rgb * 0.12;
    sum += texture(s, vec2(tc.x - blurWidth, tc.y - blurHeight)).rgb * 0.15;
    sum += texture(s, vec2(tc.x, tc.y)).rgb * 0.16;
    sum += texture(s, vec2(tc.x + blurWidth, tc.y + blurHeight)).rgb * 0.15;
    sum += texture(s, vec2(tc.x + 2.0*blurWidth, tc.y + 2.0*blurHeight)).rgb * 0.12;
    sum += texture(s, vec2(tc.x + 3.0*blurWidth, tc.y + 3.0*blurHeight)).rgb * 0.09;
    sum += texture(s, vec2(tc.x + 4.0*blurWidth, tc.y + 4.0*blurHeight)).rgb * 0.05;
    return sum;
}

vec3 blurDiagonal2(sampler2D s, vec2 tc, float blurWidth, float blurHeight) {
    vec3 sum = vec3(0.0);
    sum += texture(s, vec2(tc.x + 4.0*blurWidth, tc.y - 4.0*blurHeight)).rgb * 0.05;
    sum += texture(s, vec2(tc.x + 3.0*blurWidth, tc.y - 3.0*blurHeight)).rgb * 0.09;
    sum += texture(s, vec2(tc.x + 2.0*blurWidth, tc.y - 2.0*blurHeight)).rgb * 0.12;
    sum += texture(s, vec2(tc.x + blurWidth, tc.y - blurHeight)).rgb * 0.15;
    sum += texture(s, vec2(tc.x, tc.y)).rgb * 0.16;
    sum += texture(s, vec2(tc.x - blurWidth, tc.y + blurHeight)).rgb * 0.15;
    sum += texture(s, vec2(tc.x - 2.0*blurWidth, tc.y + 2.0*blurHeight)).rgb * 0.12;
    sum += texture(s, vec2(tc.x - 3.0*blurWidth, tc.y + 3.0*blurHeight)).rgb * 0.09;
    sum += texture(s, vec2(tc.x - 4.0*blurWidth, tc.y + 4.0*blurHeight)).rgb * 0.05;
    return sum;
}

vec3 blur(sampler2D s, vec2 tc, float blurWidth, float blurHeight) {
    return (blurHorizontal(s, tc, blurWidth) + blurVerticle(s, tc, blurHeight) + blurDiagonal1(s, tc, blurWidth, blurHeight) + blurDiagonal2(s, tc, blurWidth, blurHeight)) / 3.0;
}

void main() {
    float blurWidth = blurAmount / width;
    float blurHeight = blurAmount / height;

    vec3 color = blur(tex0, v_TexCoords, blurWidth, blurHeight);

    out_Color = vec4(color, 1.0);
    out_Color += texture(tex0, v_TexCoords);

    out_Color /= 2.0;
}