#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D at;
uniform sampler2D bt;
uniform sampler2D imgt;

void main()
{
    vec4 a = texture(at, vec2(texPos.x, 1-texPos.y));
    vec4 b = texture(bt, vec2(texPos.x, 1-texPos.y));
    vec4 img = texture(imgt, vec2(texPos.x, 1-texPos.y));

    vec4 f = a * img + b;

    fragColor = vec4(f.r, f.g, f.b, 1.0f);
}