#version 330 core
//in vec3 fragmentColor;
in vec2 UV;
in vec3 Normal;
out vec4 fragColor;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

void main() {
    vec4 diffuse = texture(diffuseTex, UV);
    vec4 specular = texture(specularTex, UV);

    fragColor = diffuse + 0.2*specular;
}