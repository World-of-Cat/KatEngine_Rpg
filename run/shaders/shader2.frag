#version 430 core

in vec2 fUV;
in vec4 fTint;
in vec3 fNormal;
in vec4 fPosition;

out vec4 colorOut;

uniform sampler2D uTexture;

void main() {
    colorOut = texture(uTexture, fUV);
//    colorOut = vec4(fUV, 1.0f, 1.0f);
}