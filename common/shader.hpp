#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

GLuint LoadShaders(std::string vertex_file,std::string fragment_file);
GLuint LoadExternalShaders(const char * vertex_file_path,const char * fragment_file_path);

class Shader
{
public:
    GLuint handle;
    Shader(std::string vertex_file,std::string fragment_file);
    Shader(const char * vertex_file_path,const char * fragment_file_path);
    ~Shader();
    void use();
};

#endif