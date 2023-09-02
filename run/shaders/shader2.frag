#version 430 core

in vec2 fUV;
in vec4 fTint;
in vec3 fNormal;
in vec4 fPosition;

out vec4 colorOut;
uniform vec3 uLightPos;

uniform sampler2D uTexture;


void main() {

    vec3 normalDir = -normalize((texture(uTexture, vec2(fUV.x + 0.5, fUV.y)).xyz * 2.0f) - 1.0f);
    vec3 lightDir = normalize(fPosition.xyz - uLightPos);

    float intensity = clamp(dot(normalDir, lightDir), 0.0, 1.0);

    colorOut = texture(uTexture, fUV) * (intensity + 0.4);
    colorOut.a = texture(uTexture, fUV).a;
//    colorOut = texture(uTexture, fUV);

}