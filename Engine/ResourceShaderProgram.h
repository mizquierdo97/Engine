#ifndef _RESOURCE_SHADER_PROGRAM
#define _RESOURCE_SHADER_PROGRAM

#include "Resource.h"
#include "ResourceTexture.h"
#include "ShaderProgram.h"
#include "Shader.h"

class ResourceShaderProgram : public Resource {

public:

	ResourceShaderProgram() {};
	ResourceShaderProgram(UUID uid) {
		uuid = uid;
	};

	~ResourceShaderProgram() {};

	bool LoadInMemory() {
		return false;
	};
	bool EraseInMemory() {
		//TODO
		return false;
	};
	bool UpdateRes() {
		return false;
	};



public:
	ShaderProgram* res_shader = nullptr;
	ResourceTexture* res_texture = nullptr;

};



#endif
