#include "Application.h"
#include "ResourceShader.h"
#include "ModuleResourceManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Glew\include\glew.h"


ShaderProgram::ShaderProgram()
{
	mProgramID = NULL;
}

ShaderProgram::~ShaderProgram()
{
	freeProgram();
}

void ShaderProgram::freeProgram()
{
	glDeleteProgram(mProgramID);
}

bool ShaderProgram::bind()
{
	glUseProgram(mProgramID);
	
	GLenum error = glGetError();

	if (error != GL_NO_ERROR){

		printf("Error binding shader! %s\n", gluErrorString(error));
		printProgramLog(mProgramID);
		return false;
	}

	return true;
}

void ShaderProgram::unbind()
{
	glUseProgram(NULL);
}

uint ShaderProgram::getProgramID()
{
	return mProgramID;
}

void ShaderProgram::printProgramLog(uint program)
{

	if (glIsProgram(program)){
		
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		
		char* infoLog = new char[maxLength];		
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);

		if (infoLogLength > 0){

			LOG("%s\n", infoLog);
		}
		
		delete[] infoLog;
	}

	else{

		printf("Name %d is not a program\n", program);
	}
}

void ShaderProgram::printShaderLog(uint shader)
{
	
	if (glIsShader(shader)){
		
		int infoLogLength = 0;
		int maxLength = infoLogLength;
				
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		
		char* infoLog = new char[maxLength];		
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

		if (infoLogLength > 0){
			
			LOG("%s\n", infoLog);
		}
		
		delete[] infoLog;
	}

	else{

		printf("Name %d is not a shader\n", shader);
	}
}

bool ShaderProgram::loadProgram(uint VertexID, uint FragmentID)
{
	
	GLint programSuccess = GL_TRUE;
	
	mProgramID = glCreateProgram();		
	
	GLuint vertexShader = VertexID;
	glAttachShader(mProgramID, vertexShader);

	GLuint fragmentShader = FragmentID;
	glAttachShader(mProgramID, fragmentShader);	
	glLinkProgram(mProgramID);	
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);

	if (programSuccess != GL_TRUE)
	{
		printProgramLog(mProgramID);
		return false;
	}

	vertexID = vertexShader;
	fragmentID = fragmentShader;
	
	return true;
}

bool ShaderProgram::loadProgram(char * Vertex_buffer, char * Fragment_buffer)
{
	
	GLint programSuccess = GL_TRUE;
	
	mProgramID = glCreateProgram();
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);	
	const GLchar* vertexShaderSource[] = { Vertex_buffer };	
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);

	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		return false;
	}
	
	glAttachShader(mProgramID, vertexShader);
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource[] = { Fragment_buffer };
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);

	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		printShaderLog(fragmentShader);
		return false;
	}

	glAttachShader(mProgramID, fragmentShader);
	glLinkProgram(mProgramID);
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);

	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", mProgramID);
		printProgramLog(mProgramID);
		return false;
	}

	vertexID = vertexShader;
	fragmentID = fragmentShader;



	return true;
}

bool ShaderProgram::UpdateShaderProgram(uint VertexID, uint FragmentID)
{
	
	GLint programSuccess = GL_TRUE;

	
	if (VertexID != 0) {

		GLuint vertexShader = VertexID;
		
		glDetachShader(mProgramID, vertexID);		
		glAttachShader(mProgramID, vertexShader);

		vertexID = vertexShader;
	}

	if (FragmentID != 0) {

		GLuint fragmentShader = FragmentID;

		glDetachShader(mProgramID, fragmentID);
		glAttachShader(mProgramID, fragmentShader);

		fragmentID = fragmentShader;
	}
	
	glLinkProgram(mProgramID);
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);

	if (programSuccess != GL_TRUE){

		printf("Error linking program %d!\n", mProgramID);
		printProgramLog(mProgramID);

		return false;
	}

	return true;
}

bool ShaderProgram::UpdateShaderProgram(std::string vs_path, std::string fs_path)
{
	uint vs_ID = 0;
	uint fs_ID = 0;

	if (strcmp(vs_path.c_str(), "")) {

		UUID v_res_uuid = App->resources->Find(vs_path.c_str());
		ResourceShader* v_res = (ResourceShader*)App->resources->Get(v_res_uuid);
		vs_ID = v_res->res_shader->shader_id;
	}

	if (strcmp(fs_path.c_str(), "")) {

		UUID f_res_uuid = App->resources->Find(fs_path.c_str());
		ResourceShader* f_res = (ResourceShader*)App->resources->Get(f_res_uuid);
		fs_ID = f_res->res_shader->shader_id;
	}
		
	UpdateShaderProgram(vs_ID, fs_ID);

	return true;
}
