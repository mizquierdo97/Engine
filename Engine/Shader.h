#pragma once
#include "Globals.h"

class Shader {
public:
	Shader();

	virtual ~Shader();


	virtual bool loadProgram() = 0;


	virtual void freeProgram();


	bool bind();


	void unbind();


	uint getProgramID();

protected:
	void printProgramLog(uint program);


	void printShaderLog(uint shader);


	//Program ID
	uint mProgramID;
};