#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D corrt;
uniform sampler2D meant;

void main()
{
    vec3 corr = vec3(texture(corrt, texPos));
    vec3 mean = vec3(texture(meant, texPos));
    vec3 cv = corr - mean * mean;
    vec3 alpha = cv/(cv+0.01);
    fragColor = vec4(alpha.r, alpha.g, alpha.b, 1.0f);
}