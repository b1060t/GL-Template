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

enum class TextureType
{
    Diffuse,
    Specular,
    Normal
};

class Element
{
private:
    Shader* _shader;
    GLuint _buffer_handle;
    GLfloat* _buffer;
    GLint* _indices;
    GLsizei _buffer_size;
    GLsizei _indices_size;
    GLuint _vao;
    GLuint _ibo;
    std::vector<Attr> _attr;

    GLuint _diffuse_texture;
    GLuint _specular_texture;
    GLuint _normal_texture;

    GLint _vert_num;
    GLint _attr_num;

    glm::mat4 _model;

public:
    Element(GLfloat* data, GLint* indices, GLsizei buffer_size, GLsizei indices_size, std::vector<Attr> attr, mango::Bitmap* bitmap);
    Element(GLfloat* data, GLint* indices, GLsizei buffer_size, GLsizei indices_size, std::vector<Attr> attr, GLuint texture);
    Element(const aiMesh* mesh, std::vector<Attr> attr, GLuint texture);
    ~Element();
    void initialzeVAO();
    void attachShader(Shader* shader);
    void render(glm::mat4 View, glm::mat4 Projection);
    void addTexture(TextureType type, mango::Bitmap* bitmap);
    void addTexture(TextureType type, GLuint tex);
};


#endif