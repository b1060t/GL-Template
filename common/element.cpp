#include <element.hpp>

Element::Element(GLfloat* data, GLsizei size, std::vector<Attr> attr, mango::Bitmap* bitmap, Shader* shader)
{
    _buffer = data;
    _buffer_size = size;
    _attr = attr;
    _bitmap = bitmap;
    _shader = shader;
    
    glGenBuffers(1, &_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, _buffer_size, _buffer, GL_STATIC_DRAW);

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _bitmap->width, _bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, _bitmap->image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenVertexArrays(1, &_vao);
    initialzeVAO();
}

Element::~Element()
{
    glDeleteBuffers(1, &_buffer_handle);
    glDeleteVertexArrays(1, &_vao);
}

void Element::initialzeVAO()
{
    glBindVertexArray(_vao);
    _attr_num = 0;
    for (size_t i = 0; i < _attr.size(); i++)
    {
        Attr a = _attr[i];
        glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
        glVertexAttribPointer(
            i, a.size, a.type, a.norm, a.stride * sizeof(_buffer[0]), (void*)(a.offset * sizeof(_buffer[0]))
        );
        _attr_num += a.size;
    }
    _vert_num = _buffer_size / (_attr_num * sizeof(_buffer[0]));
}

void Element::render(glm::mat4 View, glm::mat4 Projection)
{
    _shader->use();
    _texture_id = glGetUniformLocation(_shader->handle, "textureSampler");
    _mvp_id = glGetUniformLocation(_shader->handle, "MVP");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(_texture_id, 0);

    glBindVertexArray(_vao);

    _model = glm::mat4(1.0f);

    glm::mat4 mvp = Projection * View * _model;
    glUniformMatrix4fv(_mvp_id, 1, GL_FALSE, &mvp[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, _vert_num);
}