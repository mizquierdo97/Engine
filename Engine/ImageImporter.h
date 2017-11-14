#pragma once
#include <string>
#include "Texture.h"

class ImageImporter {
public:
	ImageImporter() {

	}
	~ImageImporter() {

	}
	
	bool ImportImage(const char* name, std::string*, bool force = false);
	bool loadTextureFromFile(const char* path, Texture**, bool is_texture = true);
	bool UpdateTextureFromFile(const char* path, Texture**, bool is_texture = true);
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height, Texture**);

public:
	
};