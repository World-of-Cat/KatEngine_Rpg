#type frag
#version 430 core

in vec2 fUV;
in vec4 fTint;
in vec3 fNormal;

out vec4 colorOut;

void main() {
    colorOut = fTint;
}