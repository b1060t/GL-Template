#version 330 core
//in vec3 fragmentColor;
in vec2 UV;
out vec3 color;
uniform sampler2D textureSampler;
void main() {
    color = texture(textureSampler, UV).rgb;
}