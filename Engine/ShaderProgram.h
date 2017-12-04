#ifndef _SHADER_PROGRAM
#define _SHADER_PROGRAM

#include "Globals.h"


class ShaderProgram {
public:
	ShaderProgram();

	virtual ~ShaderProgram();


	virtual bool loadProgram(uint VertexID, uint FragmentID);

	//virtual uint loadShaderFromFile(std::string path, ShaderType type);


	virtual void freeProgram();


	bool bind();


	void unbind();


	uint getProgramID();

protected:
	void printProgramLog(uint program);


	void printShaderLog(uint shader);


	//Program ID
	uint mProgramID = 0;
	uint fragmentID = 0;
	uint vertexID = 0;
	//ShaderType type = ShaderType::null_shader;
};

#endif