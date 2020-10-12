#pragma once
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
	using Path = std::filesystem::path;
	int width = 0;
	int height = 0; 
	int bpp = 0;
	unsigned char* pixels = nullptr;
public:
	Texture(Path filepath){
		load(filepath);
	}

	~Texture() { unload();	}

	bool load(Path filepath) noexcept {
		unload();		
		auto path = filepath.string();
		pixels = stbi_load(path.c_str(), &width, &height, &bpp, 0);
		if (!pixels) {
			return false;
		}
		return true;
	}

	void unload() noexcept {
		if (nullptr != pixels) {
			stbi_image_free(pixels);
			pixels = nullptr;
		}
	}

	int getComponents() const noexcept {
		return bpp;
	}

	int getWidth() const noexcept {
		return width;
	}

	int getHeight() const noexcept {
		return height;
	}

	unsigned char* getPixels() const noexcept {
		return pixels;
	}
};