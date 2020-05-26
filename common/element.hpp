#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <shader.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <mango/mango.hpp>


struct Attr
{
    GLint size;
    GLenum type;
    GLboolean norm;
    GLsizei stride;
    GLsizei offset;
};


class Element
{
private:
    Shader* _shader;
    GLuint _buffer_handle;
    GLfloat* _buffer;
    GLsizei _buffer_size;
    GLuint _vao;
    GLuint _vbo;
    std::vector<Attr> _attr;
    GLuint _mvp_id;
    GLuint _texture_id;
    GLuint _texture;
    mango::Bitmap* _bitmap;

    GLint _vert_num;
    GLint _attr_num;

    glm::vec3 _pos;
    glm::vec3 _scale;
    glm::vec3 _rotation;

    glm::mat4 _model;

public:
    Element(GLfloat* data, GLsizei size, std::vector<Attr> attr, mango::Bitmap* bitmap, Shader* shader);
    ~Element();
    void initialzeVAO();
    void render(glm::mat4 View, glm::mat4 Projection);
};


#endif