#ifndef _SHADER_IMPORTER
#define _SHADER_IMPORTER

#include <string>
#include "Shader.h"


class ShaderImporter {
public:
	ShaderImporter() {

	}
	~ShaderImporter() {

	}

	bool ImportShader(const char* name, std::string path,ShaderType type, bool force = false);
	
	uint loadShaderFromFile(std::string path, ShaderType type);
	
	/*bool loadTextureFromFile(const char* path, Texture**, bool is_texture = true);
	bool UpdateTextureFromFile(const char* path, Texture**, bool is_texture = true);
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height, Texture**);*/

public:

};

#endif