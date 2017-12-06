#include "Application.h"
#include "ResourceShader.h"
#include "ShadersManager.h"

ShadersManager::ShadersManager(bool start_enabled) : Module(start_enabled)
{

}

bool ShadersManager::Init()
{


	return true;
}

bool ShadersManager::Start()
{
	CreateDefaultShader();
	CreateShader("Assets/VertexShader.vrsh", "Assets/Shader.frsh", "shader_test");
	return true;
}

bool ShadersManager::CleanUp()
{
	return true;
}

update_status ShadersManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ShadersManager::Update(float dt)
{
	std::vector<ShaderProgram*>::iterator item = shader_list.begin();
	static float time_dt = 0;
	time_dt += dt;
	while (item != shader_list.end()) {
		(*item)->bind();

		//TIME
		
		GLint timeLoc = glGetUniformLocation((*item)->mProgramID, "_time");
		glUniform1f(timeLoc, sin(time_dt));

		(*item)->unbind();


		item++;
	}

	return UPDATE_CONTINUE;
}

void ShadersManager::CreateShader(std::string vs_path, std::string fs_path, std::string name)
{
	UUID v_res_uuid = App->resources->Find(vs_path.c_str());
	ResourceShader* v_res = (ResourceShader*)App->resources->Get(v_res_uuid);
	uint vs_ID = v_res->res_shader->shader_id;

	UUID f_res_uuid = App->resources->Find(fs_path.c_str());
	ResourceShader* f_res = (ResourceShader*)App->resources->Get(f_res_uuid);
	uint fs_ID = f_res->res_shader->shader_id;

	ShaderProgram* shader_program = new ShaderProgram();
	shader_program->shader_name = name;
	shader_program->loadProgram(vs_ID, fs_ID);
	shader_list.push_back(shader_program);


}

bool ShadersManager::CreateDefaultShader()
{
	
	default_shader.shader_name = name;
	//Success flag
	GLint programSuccess = GL_TRUE;

	//Generate program
	default_shader.mProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoord;\n"
		"layout(location = 3) in vec4 color;\n"
		"out float ourTime;\n"
		"out vec4 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform float _time;\n"
		"uniform vec4 _color;\n"
		"uniform mat4 model;\n"
		"uniform mat4 viewproj;\n"
		"void main()\n"
		"{\n"
		"gl_Position = viewproj *  model * vec4(position.x,position.y,position.z, 1.0f);\n"
			"ourColor = _color;\n"
			"TexCoord = texCoord;\n"
			"ourTime = _time;\n"
		"}\n"
	};

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
		//printShaderLog(vertexShader);
		return false;
	}

	//Attach vertex shader to program
	glAttachShader(default_shader.mProgramID, vertexShader);

	//Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Get fragment source
	const GLchar* fragmentShaderSource[] =
	{
		"#version 330 core\n"
		"in vec4 ourColor;\n"
		"in float ourTime;"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D _texture;\n"
		"void main()\n"
		"{\n"
		"color = texture(_texture, TexCoord) * vec4(ourColor.r,ourColor.g,ourColor.b,ourColor.a);\n"
		"}\n"
	};

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
		//printShaderLog(fragmentShader);
		return false;
	}

	//Attach fragment shader to program
	glAttachShader(default_shader.mProgramID, fragmentShader);

	//Link program
	glLinkProgram(default_shader.mProgramID);

	//Check for errors
	glGetProgramiv(default_shader.mProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", default_shader.mProgramID);
		//printProgramLog(mProgramID);
		return false;
	}
	default_shader.fragmentID = fragmentShader;
	default_shader.vertexID = vertexShader; 
	shader_list.push_back(&default_shader);
	return true;
	
}

