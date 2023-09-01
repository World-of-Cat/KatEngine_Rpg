#type frag
#version 430 core

in vec2 fUV;
in vec4 fTint;
in vec3 fNormal;

out vec4 colorOut;

uniform float uTime;

float stepper(float v, float n) {
    return round(v * n) / n;
}

void main() {
    float t = (sin(uTime) + 1.0) / 2.666667;
    float a = fUV.x - t;
    float v = stepper(-16 * a * a + 1, 5.0);
    colorOut = vec4(v, v, v, 1.0f);
//    colorOut = vec4(fUV.xxx, 1.0f);
}