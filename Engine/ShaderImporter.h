#ifndef _SHADER_IMPORTER
#define _SHADER_IMPORTER

#include <string>
#include "Shader.h"
#include "Resource.h"

class ShaderImporter {
public:
	ShaderImporter() {

	}
	~ShaderImporter() {

	}

	bool ImportShader(const char* name,Resource::Type type, bool force = false);
	
	uint loadShaderFromFile(std::string path, ShaderType type);
	uint CreateResourceFromMaterial(std::string path);
	/*bool loadTextureFromFile(const char* path, Texture**, bool is_texture = true);
	bool UpdateTextureFromFile(const char* path, Texture**, bool is_texture = true);
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height, Texture**);*/

public:

};

#endif