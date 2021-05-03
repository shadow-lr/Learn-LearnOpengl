#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

uniform float mixDegree;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    color = mix(texture(ourTexture, vec2(-TexCoord.x, TexCoord.y)),
    texture(ourTexture2, vec2(-TexCoord.x, TexCoord.y)), mixDegree);
}