#include <element.hpp>

Element::Element(GLfloat* data, GLint* indices, GLsizei buffer_size, GLsizei indices_size, std::vector<Attr> attr, mango::Bitmap* bitmap)
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

    glGenTextures(1, &_diffuse_texture);
    glBindTexture(GL_TEXTURE_2D, _diffuse_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap->width, bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap->image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenVertexArrays(1, &_vao);
    initialzeVAO();
}

Element::Element(GLfloat* data, GLint* indices, GLsizei buffer_size, GLsizei indices_size, std::vector<Attr> attr, GLuint texture)
{
    _buffer = data;
    _indices = indices;
    _buffer_size = buffer_size;
    _indices_size = indices_size;
    _attr = attr;
    _diffuse_texture = texture;

    glGenBuffers(1, &_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, _buffer_size, _buffer, GL_STATIC_DRAW);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &_vao);
    initialzeVAO();
}

Element::Element(const aiMesh* mesh, std::vector<Attr> attr, GLuint texture)
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
    _diffuse_texture = texture;

    glGenBuffers(1, &_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, _buffer_size, _buffer, GL_STATIC_DRAW);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices_size, _indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &_vao);
    initialzeVAO();
}

Element::~Element()
{
    glDeleteBuffers(1, &_buffer_handle);
    glDeleteBuffers(1, &_ibo);
    glDeleteVertexArrays(1, &_vao);
    if (_buffer != nullptr) delete _buffer; _buffer = nullptr;
    if (_indices != nullptr) delete _indices; _indices = nullptr;
}

void Element::attachShader(Shader* shader)
{
    _shader = shader;
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    _vert_num = _buffer_size / (_attr_num * sizeof(_buffer[0]));
    glBindVertexArray(0);
}

void Element::render(glm::mat4 View, glm::mat4 Projection)
{
    _shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuse_texture);
    _shader->setInt("diffuseTex", 0);

    if (_specular_texture)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _specular_texture);
        _shader->setInt("specularTex", 1);
    }
    if (_normal_texture)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _normal_texture);
        _shader->setInt("normalTex", 2);
    }

    glBindVertexArray(_vao);

    _model = glm::mat4(1.0f);
    glm::mat4 mvp = Projection * View * _model;

    _shader->setMat4("MVP", 1, GL_FALSE, &mvp[0][0]);
    glDrawElements(GL_TRIANGLES, _indices_size/sizeof(int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Element::addTexture(TextureType type, mango::Bitmap* bitmap)
{
    switch (type)
    {
    case TextureType::Specular:
        glGenTextures(1, &_specular_texture);
        glBindTexture(GL_TEXTURE_2D, _specular_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap->width, bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap->image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    case TextureType::Normal:
        glGenTextures(1, &_normal_texture);
        glBindTexture(GL_TEXTURE_2D, _normal_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap->width, bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap->image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    default:
        break;
    }
}

void Element::addTexture(TextureType type, GLuint tex)
{
    switch (type)
    {
    case TextureType::Specular:
        _specular_texture = tex;
        break;
    case TextureType::Normal:
        _normal_texture = tex;
        break;
    default:
        break;
    }
}