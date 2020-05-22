#ifndef SHADER_HPP
#define SHADER_HPP

GLuint LoadShaders(std::string vertex_file,std::string fragment_file);
GLuint LoadExternalShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif