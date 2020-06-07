#version 330 core
//in vec3 fragmentColor;
in vec2 UV;
in vec3 Normal;
out vec4 fragColor;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

uniform vec3 lightDir;

void main() {

    vec4 diffuse = texture(diffuseTex, UV);
    //vec4 specular = texture(specularTex, UV);

    float str = clamp(dot(-lightDir, Normal),0,1);
    float ambient = 0.3;
    float weight = min(str+ambient,1.0f);

    fragColor = diffuse*weight;
}