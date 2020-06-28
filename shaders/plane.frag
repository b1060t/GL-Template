#version 330 core

struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shiness;
};

struct Light
{
    vec3 pos;
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

in vec2 Tex;
in vec3 Normal;
in vec3 Pos;
out vec4 fragColor;
uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main() {

    vec3 inormal = Normal;
    inormal = normalize(inormal*2.0-1.0);

    vec4 diffuseT = texture(material.diffuse, Tex);
    vec4 specularT = vec4(material.specular, 1.0f);

    //ambient
    vec4 ambient = diffuseT * vec4(light.ambient,1.0f);
    //diffuse
    vec3 lightDir = normalize(light.pos - Pos);
    float d = clamp(dot(-lightDir, inormal),0,1);
    vec4 diffuse = diffuseT * d * vec4(light.diffuse,1.0f);
    //specular
    vec3 viewDir = normalize(viewPos - Pos);
    vec3 halfDir = normalize(-lightDir + viewDir);
    //vec3 reflectDir = reflect(lightDir,normal);
    float spec = pow(max(dot(viewDir, halfDir), 0.0), material.shiness);
    vec4 specular = spec * specularT * vec4(light.specular,1.0f);
    
    fragColor = specular + diffuse + ambient;
}