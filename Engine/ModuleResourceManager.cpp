#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager(bool start_enabled) : Module(start_enabled)
{

}

bool ModuleResourceManager::Init()
{
	return true;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}
