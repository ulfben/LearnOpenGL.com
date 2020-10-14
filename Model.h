#pragma once
#include "glad/glad.h"
#include <vector>
class Shader;
class Texture;
/*
Tutorial code puts all of the initialization and rendering straight into main. 
This class is a first attempt at organizing the OpenGL data initialization (and potentially rendering), someplace other than main(). 
I fully expect to refactor this many many times before a) I'm done with the tutorial and b) I've figured out how to organize buffers / models / textures / entities.
*/

enum Stride : GLsizei {
    XYZ = 3 * sizeof(float),
    XYZRGB = 6 * sizeof(float),
    XYZRGBST = 8 * sizeof(float)
};

namespace Components {
constexpr GLint RGB = 3;
constexpr GLint XYZ = 3;
constexpr GLint ST = 2;
};

class Model {
    const GLuint vertLocation = 0; //TODO: query shader for the attribute locations
    const GLuint colorLocation = 1;
    const GLuint textureLocation = 2;
    const GLsizei stride = Stride::XYZRGBST; //TODO: try detecting from data.
    using Buffer = std::vector<GLfloat>; //TODO: replace std::vector with std::span or std::array_view
    using IndexBuffer = std::vector<GLuint>;

    //delegated constructors made private to avoid creation of incomplete Models, until I've figured out how to organize the rendering context better.    
    explicit Model(Shader* shader);
    Model(const Buffer& vertices, Shader* shader);
    Model(const Buffer& vertices, const IndexBuffer& indices, Shader* shader);

public:
    Shader* _shader = nullptr;
    GLuint _texture;
    GLuint _vbo = GL_INVALID_VALUE; //Vertex Buffer Object
    GLuint _ebo = GL_INVALID_VALUE; //Element Buffer Object (indexes for the vertex buffer)
    GLuint _vao = GL_INVALID_VALUE; //Vertex Array Object (attribute calls, or rendering context)
    GLenum _drawType = GL_STATIC_DRAW;
    GLsizei _drawCount = 0;

    Model(const Buffer& vertices, const IndexBuffer& indices, Shader* shader, Texture* texture);
    ~Model();
    void render() const noexcept; //TODO: assumes Model is always fullý constructed (indexed and textured)
};
