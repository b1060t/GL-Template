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
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(handle, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(handle, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(handle, name.c_str()), value); 
    }
    void setMat4(const std::string &name, GLsizei count, GLboolean transpose, GLfloat* value) const
    { 
        glUniformMatrix4fv(glGetUniformLocation(handle, name.c_str()), count, transpose, value); 
    } 
};

#endif