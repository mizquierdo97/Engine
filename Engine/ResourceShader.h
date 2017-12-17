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
	Shader* res_shader;

};



#endif
