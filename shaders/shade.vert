#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 UV;
out vec3 Normal;
out vec3 Pos;

void main()
{
    mat4 MVP = Projection * View * Model;
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    UV = vertexUV;
    Normal = mat3(transpose(inverse(Model)))*vertexNorm;
    Pos = vec3(Model*vec4(vertexPosition_modelspace,1.0));
}