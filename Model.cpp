#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include <cassert>

template <class T>
inline void myBufferData(GLenum target, const std::vector<T>& v, GLenum usage) noexcept
{
    glBufferData(target, v.size() * sizeof(T), v.data(), usage);
}

Model::Model(Shader* shader)
    : _shader(shader)
{
    assert(shader != nullptr && "Please provide an adress to a valid Shader");
    glGenVertexArrays(1, &_vao); //create a rendering context
    glBindVertexArray(_vao);
};

Model::Model(const Buffer& vertices, Shader* shader)
    : Model(shader)
{
    glGenBuffers(1, &_vbo); //copy our verts into an opengl buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    myBufferData(GL_ARRAY_BUFFER, vertices, _drawType);

    const void* vertsOffset = 0;
    glVertexAttribPointer(vertLocation, Components::XYZ, GL_FLOAT, GL_FALSE, stride, vertsOffset);
    glEnableVertexAttribArray(vertLocation);
};

Model::Model(const Buffer& vertices, const IndexBuffer& indices, Shader* shader)
    : Model(vertices, shader)
{
    _drawCount = static_cast<GLsizei>(indices.size());
    glGenBuffers(1, &_ebo); //if we were provided indices, also enable the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    myBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, _drawType);

    const void* colorOffset = (void*)(Stride::XYZ); //point us past the verts
    glVertexAttribPointer(colorLocation, Components::RGB, GL_FLOAT, GL_FALSE, stride, colorOffset);
    glEnableVertexAttribArray(colorLocation);
};

Model::Model(const Buffer& vertices, const IndexBuffer& indices, Shader* shader, Texture* texture)
    : Model(vertices, indices, shader)
{
    assert(texture != nullptr && "Please provide an adress to a valid Texture");
    _texture = texture->ID;
    const void* textureOffset = (void*)(Stride::XYZRGB); //point us past the verts and colors
    glVertexAttribPointer(textureLocation, Components::ST, GL_FLOAT, GL_FALSE, stride, textureOffset);
    glEnableVertexAttribArray(textureLocation);
}

Model::~Model()
{
    _shader = nullptr;
    if (_vao != GL_INVALID_VALUE)
        glDeleteVertexArrays(1, &_vao);
    if (_vbo != GL_INVALID_VALUE)
        glDeleteBuffers(1, &_vbo);
    if (_ebo != GL_INVALID_VALUE)
        glDeleteBuffers(1, &_ebo);    
}

void Model::render() const noexcept
{ //TODO: assumes Model is always fullý constructed (indexed and textured)
    glBindVertexArray(_vao); //TODO: wasteful if many Models share the same _vao and _shader. Perhaps create a Material class and sort models on that before rendering?
    _shader->use();
    glBindTexture(GL_TEXTURE_2D, _texture);
    glDrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, 0);
}
