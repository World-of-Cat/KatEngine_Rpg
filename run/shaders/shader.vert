#version 430 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec2 vTexCoord;
layout(location=2) in vec4 vTint;
layout(location=3) in vec3 vNormal;

out vec4 fTint;
out vec3 fNormal;
out vec2 fUV;

void main() {
    gl_Position = vec4(vPosition, 1.0);

    fUV = vTexCoord;
    fTint = vTint;
    fNormal = vNormal;
}