#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D tex;

uniform float width;
uniform float height;

void main()
{
    fragColor = texture(tex, texPos);
}