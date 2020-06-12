#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D tex;

uniform float width;
uniform float height;

float xoffset = 1.0 / width;
float yoffset = 1.0 / height;

int radius = 5;

vec4 getVal(float x, float y)
{
    return texture(tex,vec2(clamp(x,0,width), clamp(y,0,height)));
}

void main()
{
    float r = 0;
    float g = 0;
    float b = 0;
    for(int i=-radius;i<radius+1;i++)
    {
        for(int j=-radius;j<radius+1;j++)
        {
            vec4 color = getVal(texPos.x+i*xoffset,texPos.y+j*yoffset);
            r += color.r;
            g += color.g;
            b += color.b;
        }
    }
    vec3 mean = vec3(r, g, b);
    mean /= (2*radius+1)*(2*radius+1);
    fragColor = vec4(mean.r, mean.g, mean.b, 1.0f);
}