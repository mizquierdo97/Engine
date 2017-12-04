#ifndef _SHADER
#define _SHADER
#include "Globals.h"

enum ShaderType {
	vertex_shader,
	pixel_shader,
	null_shader,
};

class Shader {

public:
	Shader() {

	}
	~Shader() {

	}

public:
	uint shader_id;

};


#endif