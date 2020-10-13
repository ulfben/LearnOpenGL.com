#include "Texture.h"
#include "defines.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
DISABLE_WARNING_PUSH
DISABLE_WARNING_USE_GSL_AT
DISABLE_WARNING_USE_GSL_FINALLY
#include "stb_image.h"
DISABLE_WARNING_POP

Texture::Texture(std::filesystem::path file)
{
    auto path = file.string();
    _pixels = stbi_load(path.c_str(), &_width, &_height, &_bpp, 0);
    if (!_pixels) {
        throw std::runtime_error(file.filename().string() + "failed to load\n");
    }
}

Texture::~Texture() {
    if (nullptr != _pixels) {
        stbi_image_free(_pixels);
        _pixels = nullptr;
    }
}

int Texture::components() const noexcept
{
    return _bpp;
}

int Texture::width() const noexcept
{
    return _width;
}

int Texture::height() const noexcept
{
    return _height;
}

unsigned char* Texture::pixels() const noexcept
{
    return _pixels;
}