
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

	GLuint vertexShader = VertexID;//loadShaderFromFile("Assets/VertexShader.txt", ShaderType::vertex_shader);
	//GLuint vertexShader = VertexID;
	//Attach vertex shader to program
	glAttachShader(mProgramID, vertexShader);
	
	GLuint fragmentShader = FragmentID;// loadShaderFromFile("Assets/Shader.txt", ShaderType::pixel_shader);
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
