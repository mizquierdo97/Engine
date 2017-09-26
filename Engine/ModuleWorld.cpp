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
	return false;
}

update_status ModuleWorld::PreUpdate(float dt)
{
	return update_status();
}

update_status ModuleWorld::Update(float dt)
{
	return update_status();
}

update_status ModuleWorld::PostUpdate(float dt)
{
	return update_status();
}

bool ModuleWorld::CleanUp()
{
	return false;
}
