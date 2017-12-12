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
			LOG("%s\n", infoLog);
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
			LOG("%s\n", infoLog);
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

	
	//Get vertex source

	GLuint vertexShader = VertexID;
	
	//Attach vertex shader to program
	glAttachShader(mProgramID, vertexShader);
	

	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSource[] =
	{
		//GEOMETRY SHADER
		"#version 330\n"
		"#extension GL_ARB_geometry_shader4 : enable\n"

		"in float ourTime[];\n"
		"out float GTime;\n"
		"in vec3 pos[];\n"
		"out vec3 GPos;\n"
		"in vec4 ourColor[];\n"
		"out vec4 GColor;\n"
		"in vec3 ourNormal[];\n"
		"out vec3 GNormal;\n"
		"layout(triangles) in;\n"
		"layout(triangle_strip, max_vertices = 3) out;\n"
		"uniform "
		///////////////////////
		"void main()\n"
		"{\n"


		"int i;\n"
		"vec4 vertex;\n"
		"vec3 p1 = (gl_PositionIn[1] - gl_PositionIn[0]).xyz;\n"
		"vec3 p2 = (gl_PositionIn[2] - gl_PositionIn[0]).xyz;\n"
		"vec3 X = normalize(p1);\n"
		"vec3 Y = normalize(cross(p1,p2));\n"
		"vec3 Z = cross(X,Y);\n"
		"mat3 rot =  mat3(X.xyz,Y.xyz,Z.xyz);\n //rotation matrix\n"

		"for (i = 0; i < gl_VerticesIn; i++)\n"
		"{\n"
		"	gl_Position = gl_PositionIn[i];\n"
		"	GColor = ourColor[i];\n"
		"	GTime = ourTime[i];\n"
		"	GNormal = rot *ourNormal[i];\n"
		"	GPos = pos[i];\n"
		"	EmitVertex();\n"
		"}\n"
		"EndPrimitive();\n"
		//New piece of geometry!
		"for (i = 0; i < gl_VerticesIn; i++)\n"
		"{\n"
		"	vertex = gl_PositionIn[i];\n"
		"	vertex.z = -vertex.z;\n"
		"	gl_Position = vertex;\n"
		"GColor =  ourColor[i];\n"
		"	EmitVertex();\n"
		"}\n"
		"EndPrimitive();\n"
		"}\n"
	};



	//Set fragment source
	glShaderSource(geometryShader, 1, geometryShaderSource, NULL);

	//Compile fragment source
	glCompileShader(geometryShader);

	//Check fragment shader for errors
	GLint gShaderCompiled = GL_FALSE;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &gShaderCompiled);
	if (gShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile geometry shader %d!\n", geometryShader);
		printShaderLog(geometryShader);
		return false;
	}

	glAttachShader(mProgramID, geometryShader);


	GLuint fragmentShader = FragmentID;
		
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
	vertexID = vertexShader;
	fragmentID = fragmentShader;
	geometryID = geometryShader;
	return true;
}

bool ShaderProgram::loadProgram(char * Vertex_buffer, char * Fragment_buffer)
{
	//Success flag
	GLint programSuccess = GL_TRUE;

	//Generate program
	mProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] = { Vertex_buffer };

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		return false;
	}

	//Attach vertex shader to program
	glAttachShader(mProgramID, vertexShader);


	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSource[] =
	{
		//GEOMETRY SHADER
		"#version 330\n"
		"#extension GL_ARB_geometry_shader4 : enable\n"

		"in float ourTime[];\n"
		"out float GTime;\n"
		"in vec3 pos[];\n"
		"out vec3 GPos;\n"
		"in vec4 ourColor[];\n"
		"out vec4 GColor;\n"
		"in vec3 ourNormal[];\n"
		"out vec3 GNormal;\n"
		"layout(triangles) in;\n"
		"layout(triangle_strip, max_vertices = 3) out;\n"
		"uniform "
		///////////////////////
		"void main()\n"
		"{\n"
		

		"int i;\n"
		"vec4 vertex;\n"
		"vec3 p1 = (gl_PositionIn[1] - gl_PositionIn[0]).xyz;\n"
		"vec3 p2 = (gl_PositionIn[2] - gl_PositionIn[0]).xyz;\n"
		"vec3 X = normalize(p1);\n"
		"vec3 Y = normalize(cross(p1,p2));\n"
		"vec3 Z = cross(X,Y);\n"
		"mat3 rot =  mat3(X.xyz,Y.xyz,Z.xyz);\n //rotation matrix\n"
		
		"for (i = 0; i < gl_VerticesIn; i++)\n"
		"{\n"
		"	gl_Position = gl_PositionIn[i];\n"
		"	GColor = ourColor[i];\n"
		"	GTime = ourTime[i];\n"
		"	GPos = pos[i];\n"
		"	GNormal = rot * ourNormal[i];\n"
		"	EmitVertex();\n"
		"}\n"
		"EndPrimitive();\n"
		//New piece of geometry!
		"for (i = 0; i < gl_VerticesIn; i++)\n"
		"{\n"
		"	vertex = gl_PositionIn[i];\n"
		"	vertex.z = -vertex.z;\n"
		"	gl_Position = vertex;\n"
		"GColor =  ourColor[i];\n"
		"	EmitVertex();\n"
		"}\n"
		"EndPrimitive();\n"
		"}\n"
	};



	//Set fragment source
	glShaderSource(geometryShader, 1, geometryShaderSource, NULL);

	//Compile fragment source
	glCompileShader(geometryShader);

	//Check fragment shader for errors
	GLint gShaderCompiled = GL_FALSE;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &gShaderCompiled);
	if (gShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile geometry shader %d!\n", geometryShader);
		printShaderLog(geometryShader);
		return false;
	}

	glAttachShader(mProgramID, geometryShader);

	//Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Get fragment source
	const GLchar* fragmentShaderSource[] = { Fragment_buffer };
	//Set fragment source
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	//Compile fragment source
	glCompileShader(fragmentShader);

	//Check fragment shader for errors
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		printShaderLog(fragmentShader);
		return false;
	}

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

bool ShaderProgram::UpdateShader(uint VertexID, uint FragmentID)
{
	//Success flag
	GLint programSuccess = GL_TRUE;

	//Generate program
	
	//Detach actual shaders

	
	//Get vertex source
	if (VertexID != 0) {
		GLuint vertexShader = VertexID;
		glDetachShader(mProgramID, vertexID);
		//Attach vertex shader to program
		glAttachShader(mProgramID, vertexShader);
	}

	if (FragmentID != 0) {
		GLuint fragmentShader = FragmentID;

		glDetachShader(mProgramID, fragmentID);
		//Attach fragment shader to program
		glAttachShader(mProgramID, fragmentShader);
	}
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

bool ShaderProgram::UpdateShader(std::string vs_path, std::string fs_path)
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
		
	UpdateShader(vs_ID, fs_ID);
	return true;
}
