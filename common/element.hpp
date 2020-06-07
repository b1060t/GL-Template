#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <common.hpp>
#include <map>

namespace tide
{
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
    
    public:
    
        std::map<std::string, glm::vec3> vec3dic;
        std::map<std::string, glm::mat4> mat4dic;
        std::map<std::string, GLuint> texdic;
    
        Element(GLfloat* data, GLint* indices, GLsizei buffer_size, GLsizei indices_size, std::vector<Attr> attr);
        Element(const aiMesh* mesh, std::vector<Attr> attr);
        Element(const aiScene* meshs, std::vector<Attr> attr);
        ~Element();
    
        void addVec3Uniform(const std::string &name, glm::vec3 value);
        void addMat4Uniform(const std::string &name, glm::mat4 value);
    
        void initialzeVAO();
        void attachShader(Shader* shader);
        void render();
        void addTexture(std::string type, mango::Bitmap* bitmap);
        void addTexture(std::string type, GLuint tex);
    };
}

#endif