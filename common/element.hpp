#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <common.hpp>

struct Attr
{
    GLint size;
    GLenum type;
    GLboolean norm;
    GLsizei stride;
    GLsizei offset;
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 tex;
    glm::vec3 norm;
};

struct Texture
{
    GLuint id;
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
    Texture _texture;

    GLint _vert_num;
    GLint _attr_num;

    glm::vec3 _pos;
    glm::vec3 _scale;
    glm::vec3 _rotation;

    glm::mat4 _model;

public:
    Element(GLfloat* data, GLsizei size, std::vector<Attr> attr, mango::Bitmap* bitmap);
    Element(GLfloat* data, GLsizei size, std::vector<Attr> attr, Texture texture);
    ~Element();
    void initialzeVAO();
    void attachShader(Shader* shader);
    void render(glm::mat4 View, glm::mat4 Projection);
};


#endif