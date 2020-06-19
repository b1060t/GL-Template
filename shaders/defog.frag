#version 330 core

in vec2 texPos;

out vec4 fragColor[2];

uniform sampler2D I;
uniform sampler2D P;
uniform sampler2D a;
uniform sampler2D b;

uniform float width;
uniform float height;

uniform int phase;

float xoffset = 1.0 / width;
float yoffset = 1.0 / height;

int radius = 5;

vec4 getVal(sampler2D tex, float x, float y)
{
    return texture(tex,vec2(clamp(x,0,width), clamp(y,0,height)));
}

vec3 mean(sampler2D tex)
{
    float r = 0;
    float g = 0;
    float b = 0;
    for(int i=-radius;i<radius+1;i++)
    {
        for(int j=-radius;j<radius+1;j++)
        {
            vec4 color = getVal(tex, texPos.x+i*xoffset,texPos.y+j*yoffset);
            r += color.r;
            g += color.g;
            b += color.b;
        }
    }
    vec3 mean = vec3(r, g, b);
    mean /= (2*radius+1)*(2*radius+1);
    return mean;
}
vec3 gmean(sampler2D tex)
{
    float r = 0;
    float g = 0;
    float b = 0;
    for(int i=-radius;i<radius+1;i++)
    {
        for(int j=-radius;j<radius+1;j++)
        {
            vec4 color = getVal(tex, texPos.x+i*xoffset,texPos.y+j*yoffset);
            r += color.r;
            g += color.g;
            b += color.b;
        }
    }
    vec3 mean = vec3(r, g, b);
    mean /= (2*radius+1)*(2*radius+1);
    float gr = mean.r*0.299 + mean.g*0.587 + mean.b*0.114;
    return vec3(gr,gr,gr);
}

vec3 gcorr(sampler2D tex1, sampler2D tex2)
{
    float r = 0;
    float g = 0;
    float b = 0;
    for(int i=-radius;i<radius+1;i++)
    {
        for(int j=-radius;j<radius+1;j++)
        {
            vec4 color1 = getVal(tex1, texPos.x+i*xoffset,texPos.y+j*yoffset);
            vec4 color2 = getVal(tex2, texPos.x+i*xoffset,texPos.y+j*yoffset);
            float gr = color2.r*0.299 + color2.g*0.587 + color2.b*0.114;
            r += color1.r*gr;
            g += color1.g*gr;
            b += color1.b*gr;
        }
    }
    vec3 corr = vec3(r, g, b);
    corr /= (2*radius+1)*(2*radius+1);
    return corr;
}
vec3 corr(sampler2D tex1, sampler2D tex2)
{
    float r = 0;
    float g = 0;
    float b = 0;
    for(int i=-radius;i<radius+1;i++)
    {
        for(int j=-radius;j<radius+1;j++)
        {
            vec4 color1 = getVal(tex1, texPos.x+i*xoffset,texPos.y+j*yoffset);
            vec4 color2 = getVal(tex2, texPos.x+i*xoffset,texPos.y+j*yoffset);
            r += color1.r*color2.r;
            g += color1.g*color2.g;
            b += color1.b*color2.b;
        }
    }
    vec3 corr = vec3(r, g, b);
    corr /= (2*radius+1)*(2*radius+1);
    return corr;
}

vec3[2] calab(sampler2D I, sampler2D P)
{
    vec3 mI = mean(I);
    vec3 mP = gmean(P);
    vec3 cII = corr(I, I);
    vec3 cIP = gcorr(I, P);

    vec3 varI = cII - mI * mI;
    vec3 covIP = cIP - mI * mP;

    vec3 a = covIP / (varI + 0.01);
    vec3 b = mP - a * mI;
    vec3 o[2] = vec3[](a, b);
    return o;
}

vec3 calq(sampler2D a, sampler2D b, sampler2D I)
{
    vec3 ma = mean(a);
    vec3 mb = mean(b);
    vec3 o = ma * vec3(getVal(I, texPos.x, texPos.y)) + mb;
    return o;
}

int window = 11;
vec3 caldrk(sampler2D I)
{
    float m = 1;
    for(int i=-window;i<window+1;i++)
    {
        for(int j=-window;j<window+1;j++)
        {
            vec4 color = getVal(I, texPos.x+i*xoffset,texPos.y+j*yoffset);
            m = min(min(min(color.r, color.g), color.b), m);
        }
    }
    return vec3(m, m, m);
}

float t0 = 0.1;
float w = 0.8;
float alpha = 0.75;
vec3 cal(sampler2D I, sampler2D P)
{
    vec3 c = vec3(getVal(I, texPos.x,texPos.y));
    vec3 t = vec3(1.0) - (w * c) / alpha;
    vec3 p = vec3(getVal(P, texPos.x,texPos.y));
    vec3 j = (p-alpha)/max(t,vec3(t0)) + alpha;
    return j;
}

void main()
{
    switch(phase)
    {
        case 0:
            vec3 rst1[2] = calab(I, P);
            fragColor[0] = vec4(rst1[0], 1.0);
            fragColor[1] = vec4(rst1[1], 1.0);
            break;
        case 1:
            vec3 rst2 = calq(a, b, I);
            fragColor[0] = vec4(rst2, 1.0);
            break;
        case 2:
            vec3 rst3 = caldrk(I);
            fragColor[0] = vec4(rst3, 1.0);
        case 3:
            vec3 rst4 = cal(I, P);
            fragColor[0] = vec4(rst4, 1.0);
    }
}