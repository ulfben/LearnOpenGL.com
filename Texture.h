#pragma once
#include <string_view>
#include <filesystem>
#include "Image.h"
//TODO: can't be passed by value, since running the destructor == deleting a global resource. This design is too fragile.
class Texture {    
    using Path = std::filesystem::path;
    using GLuint = unsigned int;
public:
    GLuint ID;
    Texture(const Image& data);
    ~Texture();      

    int components() const noexcept;

    int width() const noexcept;

    int height() const noexcept;

    unsigned char* pixels() const noexcept;
};
