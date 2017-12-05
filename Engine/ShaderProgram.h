#ifndef _SHADER_PROGRAM
#define _SHADER_PROGRAM

#include "Globals.h"
#include <string>

class ShaderProgram {
public:
	ShaderProgram();

	~ShaderProgram();


	bool loadProgram(uint VertexID, uint FragmentID);

	bool UpdateShader(uint VertexID =0, uint FragmentID =0);

	bool UpdateShader(std::string VertexID = "", std::string FragmentID = "");

	void freeProgram();


	bool bind();


	void unbind();


	uint getProgramID();

protected:
	void printProgramLog(uint program);


	void printShaderLog(uint shader);


public:
	std::string shader_name;

	//Program ID
	uint mProgramID = 0;
	uint fragmentID = 0;
	uint vertexID = 0;
	//ShaderType type = ShaderType::null_shader;
};

#endif