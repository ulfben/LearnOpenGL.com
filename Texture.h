#pragma once
#include <filesystem>
class Texture {    
    int _width = 0;
    int _height = 0;
    int _bpp = 0;
    unsigned char* _pixels = nullptr;

public:
    Texture(std::filesystem::path file);
    ~Texture();      

    int components() const noexcept;

    int width() const noexcept;

    int height() const noexcept;

    unsigned char* pixels() const noexcept;
};
