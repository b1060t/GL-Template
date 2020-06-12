#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D meant;
uniform sampler2D at;

void main()
{
    vec3 mean = vec3(texture(meant, texPos));
    vec3 a = vec3(texture(at, texPos));
    vec3 beta = mean - a * mean;
    fragColor = vec4(beta.r, beta.g, beta.b, 1.0f);
}