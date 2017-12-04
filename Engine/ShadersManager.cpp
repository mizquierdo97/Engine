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

