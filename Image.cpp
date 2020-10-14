#include "Image.h"
#include "defines.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
DISABLE_WARNING_PUSH
DISABLE_WARNING_USE_GSL_AT
DISABLE_WARNING_USE_GSL_FINALLY
#include "stb_image.h"
DISABLE_WARNING_POP

Image::Image(std::filesystem::path file)
{
    auto path = file.string();
    _pixels = stbi_load(path.c_str(), &_width, &_height, &_bpp, 0);
    if (!_pixels) {
        throw std::runtime_error(file.filename().string() + "failed to load\n");
    }
}

Image::~Image() {
    if (nullptr != _pixels) {
        stbi_image_free(_pixels);
        _pixels = nullptr;
    }
}

int Image::components() const noexcept
{
    return _bpp;
}

int Image::width() const noexcept
{
    return _width;
}

int Image::height() const noexcept
{
    return _height;
}

unsigned char* Image::pixels() const noexcept
{
    return _pixels;
}