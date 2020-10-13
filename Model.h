#pragma once
#include "Shader.h"
#include "Texture.h"
struct Model
{
    using GLenum = unsigned int;
    using GLuint = unsigned int;
    using GLint = int;
    Shader _shader;
    Texture _texture;
    GLuint _vbo;
    GLuint _vao;
    GLenum _drawType;
    GLint _drawStart;
    GLint _drawCount;
};

