#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNorm;
//layout(location=1) in vec3 vertexColor;
uniform mat4 MVP;

//out vec3 fragmentColor;
out vec2 UV;
out vec3 Normal;

void main()
{
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    UV = vec2(vertexUV.x, vertexUV.y);
    Normal = vertexNorm;
}