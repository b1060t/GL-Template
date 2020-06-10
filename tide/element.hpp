#ifndef TIDE_ELEMENT_HPP
#define TIDE_ELEMENT_HPP

#include <external.hpp>
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

        glm::vec3 _pos = glm::vec3(0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);
        glm::vec3 _rot = glm::vec3(0.0f, 0.0f, 0.0f);
    
    public:
    
        std::map<std::string, glm::vec3> vec3dic;
        std::map<std::string, glm::mat4> mat4dic;
        std::map<std::string, GLuint> texdic;
        std::map<std::string, GLfloat> floatdic;

        GLboolean internal_model = true;
    
        Element(GLfloat* data, GLint* indices, GLsizei buffer_size, GLsizei indices_size, std::vector<Attr> attr)
        {
            _buffer = data;
            _indices = indices;
            _buffer_size = buffer_size;
            _indices_size = indices_size;
            _attr = attr;
    
            glGenBuffers(1, &_buffer_handle);
            glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
            glBufferData(GL_ARRAY_BUFFER, _buffer_size, _buffer, GL_STATIC_DRAW);
    
            glGenBuffers(1, &_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);
    
            glGenVertexArrays(1, &_vao);
            initialzeVAO();
        }
        Element(const aiMesh* mesh, std::vector<Attr> attr)
        {
            std::vector<GLfloat> vertices;
    	    std::vector<int> indices;
    
    	    const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
    	    for(unsigned int i = 0; i < mesh->mNumVertices; i++) 
    	    {
    	    	const aiVector3D* pPos = &(mesh->mVertices[i]);
    	    	const aiVector3D* pNormal = &(mesh->mNormals[i]);
    	    	const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &aiZeroVector;
    
    	    	vertices.push_back(pPos->x);
    	    	vertices.push_back(pPos->y);
    	    	vertices.push_back(pPos->z);
    	    	vertices.push_back(pTexCoord->x);
    	    	vertices.push_back(pTexCoord->y);
                vertices.push_back(pNormal->x);
    	    	vertices.push_back(pNormal->y);
    	    	vertices.push_back(pNormal->z);
    	    }
    	    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    	    {
    	    	const aiFace& face = mesh->mFaces[i];
    	    	assert(face.mNumIndices == 3);
    	    	indices.push_back(face.mIndices[0]);
    	    	indices.push_back(face.mIndices[1]);
    	    	indices.push_back(face.mIndices[2]);
    	    }
    
            _buffer = &vertices[0];
            _indices = &indices[0];
            _buffer_size = vertices.size()*sizeof(GLfloat);
            _indices_size = indices.size()*sizeof(int);
            _attr = attr;
    
            glGenBuffers(1, &_buffer_handle);
            glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
            glBufferData(GL_ARRAY_BUFFER, _buffer_size, _buffer, GL_STATIC_DRAW);
    
            glGenBuffers(1, &_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);
    
            glGenVertexArrays(1, &_vao);
            initialzeVAO();
        }
        Element(const aiScene* meshs, std::vector<Attr> attr)
        {
            std::vector<GLfloat> vertices;
            std::vector<int> indices;

            int num = meshs->mNumMeshes;
            const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
            int count = 0;
            for (int j=0; j < num; j++)
            {
                const aiMesh* mesh = meshs->mMeshes[j];
                for(unsigned int i = 0; i < mesh->mNumVertices; i++)
                {
                	const aiVector3D* pPos = &(mesh->mVertices[i]);
                	const aiVector3D* pNormal = &(mesh->mNormals[i]);
                	const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &aiZeroVector;

                	vertices.push_back(pPos->x);
                	vertices.push_back(pPos->y);
                	vertices.push_back(pPos->z);
                	vertices.push_back(pTexCoord->x);
                	vertices.push_back(pTexCoord->y);
                    vertices.push_back(pNormal->x);
                	vertices.push_back(pNormal->y);
                	vertices.push_back(pNormal->z);
                }
                for(unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    const aiFace& face = mesh->mFaces[i];
                    assert(face.mNumIndices == 3);
                    indices.push_back(face.mIndices[0]+count);
                    indices.push_back(face.mIndices[1]+count);
                    indices.push_back(face.mIndices[2]+count);
                }
                count+=mesh->mNumVertices;
            }

            _buffer = &vertices[0];
            _indices = &indices[0];
            _buffer_size = vertices.size()*sizeof(GLfloat);
            _indices_size = indices.size()*sizeof(int);
            _attr = attr;

            glGenBuffers(1, &_buffer_handle);
            glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
            glBufferData(GL_ARRAY_BUFFER, _buffer_size, _buffer, GL_STATIC_DRAW);

            glGenBuffers(1, &_ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);

            glGenVertexArrays(1, &_vao);
            initialzeVAO();
        }
        ~Element()
        {
            glDeleteBuffers(1, &_buffer_handle);
            glDeleteBuffers(1, &_ibo);
            glDeleteVertexArrays(1, &_vao);
        }
    
        void addVec3Uniform(const std::string &name, glm::vec3 value)
        {
            vec3dic.insert(std::pair<std::string, glm::vec3>(name, value));
        }
        void addMat4Uniform(const std::string &name, glm::mat4 value)
        {
            mat4dic.insert(std::pair<std::string, glm::mat4>(name, value));
        }
        void addFloatUniform(const std::string &name, GLfloat value)
        {
            floatdic.insert(std::pair<std::string, GLfloat>(name, value));
        }
    
        void initialzeVAO()
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
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
            _vert_num = _buffer_size / (_attr_num * sizeof(_buffer[0]));
            glBindVertexArray(0);
        }
        void attachShader(Shader* shader)
        {
            _shader = shader;
        }
        void render()
        {
            _shader->use();

            int texcount = 0;
            for(auto t : texdic)
            {
                glActiveTexture(GL_TEXTURE0 + texcount);
                glBindTexture(GL_TEXTURE_2D, t.second);
                _shader->setInt(t.first, texcount);
                texcount++;
            }

            for(auto v : mat4dic)
            {
                _shader->setMat4(v.first, 1, GL_FALSE, &v.second[0][0]);
            }
            for(auto v : vec3dic)
            {
                _shader->setVec3(v.first, 1, &v.second[0]);
            }
            for(auto v : floatdic)
            {
                _shader->setFloat(v.first, v.second);
            }

            if(internal_model)
            {
                glm::mat4 model = glm::mat4(1.0f);
                glm::qua<float> q(glm::radians(_rot));
                model = glm::scale(glm::mat4(1.0f), _scale) * model;
                model = glm::mat4_cast(q) * model;
                model = glm::translate(glm::mat4(1.0f), _pos) * model;
                _shader->setMat4("Model", 1, GL_FALSE, &model[0][0]);
            }

            glBindVertexArray(_vao);
            glDrawElements(GL_TRIANGLES, _indices_size/sizeof(int), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        void addTexture(std::string type, mango::Bitmap* bitmap)
        {
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap->width, bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap->image);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            texdic.insert(std::pair<std::string, GLuint>(type, tex));
        }
        void addTexture(std::string type, GLuint tex)
        {
            texdic.insert(std::pair<std::string, GLuint>(type, tex));
        }

        void setPosition(glm::vec3 pos)
        {
            _pos = pos;
        }
        void setRotation(glm::vec3 rot)
        {
            _rot = rot;
        }
        void setScale(glm::vec3 scale)
        {
            _scale = scale;
        }
    };
}

#endif