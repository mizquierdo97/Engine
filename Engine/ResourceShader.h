#ifndef _RESOURCE_SHADER
#define _RESOURCE_SHADER

#include "Resource.h"
#include "ShaderProgram.h"
#include "Shader.h"

class ResourceShader : public Resource {

public :

	ResourceShader() {};
	ResourceShader(UUID uid) {
		uuid = uid;
	};

	~ResourceShader() {};

	bool LoadInMemory();
	bool EraseInMemory() {
		//TODO
		return false;
	};



public:
	Shader res_shader;

};



#endif
