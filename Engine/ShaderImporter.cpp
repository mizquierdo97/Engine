#include "Application.h"
#include "ShaderImporter.h"
#include "ShaderProgram.h"

#include "Glew\include\glew.h"


bool ShaderImporter::ImportShader(const char * path, Resource::Type type, bool force)
{
	
	App->resources->ImportShader(path, type);
	
	return true;
}


uint ShaderImporter::loadShaderFromFile(std::string path, ShaderType type)
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
		
		ShaderLog(id);
		return 0;
	}


	return id;
}

uint ShaderImporter::CreateResourceFromMaterial(std::string path)
{
	return uint();
}
