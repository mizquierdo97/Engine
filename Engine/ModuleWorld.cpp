#include "Application.h"
#include "ModuleWorld.h"
#include "Primitives.h"
ModuleWorld::ModuleWorld(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleWorld::~ModuleWorld()
{
}

bool ModuleWorld::Init()
{
	
	return true;
}

bool ModuleWorld::Start() {
	

	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{
	if (App->input->file_dropped) {
		ImportMesh(App->input->dropped_filedir);
		App->input->file_dropped = false;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float dt)
{
	
	


	//RENDER MESHES
	std::vector<Mesh>::iterator temp = App->assimp->mesh_vector->begin();
	while (temp != App->assimp->mesh_vector->end()) {		
		App->renderer3D->Render(*temp);
		temp++;
	}

	ImGui::Render();
	

	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}

void ModuleWorld::ImportMesh(char * path)
{
	App->assimp->ImportMesh(path);
}
