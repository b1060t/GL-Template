#version 330 core
layout(location = 0) in vec2 vert_pos;
layout(location = 1) in vec2 tex_pos;

out vec2 texPos;

void main()
{
    gl_Position = vec4(vert_pos.x, vert_pos.y, 0.0f, 1.0f);
    texPos = tex_pos;
}