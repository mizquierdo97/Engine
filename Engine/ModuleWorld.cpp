#include "Application.h"
#include "ModuleWorld.h"

ModuleWorld::ModuleWorld(bool start_enable)
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
	sphere = new Sphere({ 0,0,0 }, 1);
	sphere->Triangulate(vect, norms, NULL, 6144, false);
	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{

	
	
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float dt)
{
	

	App->renderer3D->Render(vect, norms);
	if (App->input->GetKey(SDL_SCANCODE_LEFT)== KEY_DOWN) {
		sphere->pos.z += 0.1f;
	}
	
	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}
