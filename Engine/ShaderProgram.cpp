
#include "ShaderProgram.h"
#include "Shader.h"
#include "Glew\include\glew.h"


ShaderProgram::ShaderProgram()
{
	mProgramID = NULL;
}

ShaderProgram::~ShaderProgram()
{
	//Free program if it exists
	freeProgram();
}

void ShaderProgram::freeProgram()
{
	//Delete program
	glDeleteProgram(mProgramID);
}

bool ShaderProgram::bind()
{
	//Use shader
	glUseProgram(mProgramID);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error binding shader! %s\n", gluErrorString(error));
		printProgramLog(mProgramID);
		return false;
	}

	return true;
}

void ShaderProgram::unbind()
{
	//Use default program
	glUseProgram(NULL);
}

uint ShaderProgram::getProgramID()
{
	return mProgramID;
}

void ShaderProgram::printProgramLog(uint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void ShaderProgram::printShaderLog(uint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

bool ShaderProgram::loadProgram(uint VertexID, uint FragmentID)
{
	//Success flag
	GLint programSuccess = GL_TRUE;

	//Generate program
	mProgramID = glCreateProgram();

	//Create vertex shader
	//GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source

	GLuint vertexShader = 0;//loadShaderFromFile("Assets/VertexShader.txt", ShaderType::vertex_shader);
	//GLuint vertexShader = VertexID;
	//Attach vertex shader to program
	glAttachShader(mProgramID, vertexShader);
	
	GLuint fragmentShader = 0;// loadShaderFromFile("Assets/Shader.txt", ShaderType::pixel_shader);
	//GLuint fragmentShader = FragmentID;
	
	//Attach fragment shader to program
	glAttachShader(mProgramID, fragmentShader);


	//Link program
	glLinkProgram(mProgramID);

	//Check for errors
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", mProgramID);
		printProgramLog(mProgramID);
		return false;
	}

	return true;
}
/*
uint ShaderProgram::loadShaderFromFile(std::string path, ShaderType type)
{
	uint id = 0;
	switch (type) {
	case ShaderType::pixel_shader:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderType::vertex_shader:
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	}

	
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;
	std::string string_buffer;
	pFile = fopen(path.c_str(), "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = new char[lSize + 1];// (char*)malloc(sizeof(char)*lSize);
	memset(buffer, 0, lSize + 1);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);

	fclose(pFile);

	//Set fragment source
	glShaderSource(id, 1, (const GLchar* const *)&buffer, NULL);

	//Compile fragment source
	glCompileShader(id);

	//Check fragment shader for errors
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		//printf("Unable to compile fragment shader %d!\n", id);
		//printShaderLog(id);
		return 0;
	}


	return id;
}
*/