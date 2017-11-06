#include "Application.h"
#include "ModuleResourceManager.h"
#include "Globals.h"

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

UUID ModuleResourceManager::ImportFile(const char * path, bool force)
{
	Resource::Type type = TypeFromExtension(path);
		
	switch (type) {
	case Resource::texture:
		App->filesystem->ImportImage(path);
		break;

	}

	CreateNewResource(type);

	return UUID();
}

Resource * ModuleResourceManager::CreateNewResource(Resource::Type type, UUID force_uid)
{
	Resource* ret = nullptr;
	UUID uuid;

	if (force_uid != IID_NULL) {
		uuid = force_uid;
	}
	else {
		UuidCreate(&uuid);
	}

	switch (type) {
	case Resource::texture:
		//ret = (Resource*) new ResourceTexture(uid);
		break;

	}

	return nullptr;
}

Resource::Type ModuleResourceManager::TypeFromExtension(const char * path)
{
	const char* extension = GetExtension(path).c_str();

	if (!strcmp(extension, "png"))
		return Resource::texture;
	if (!strcmp(extension, "tga"))
		return Resource::texture;
	if (!strcmp(extension, "jpg"))
		return Resource::texture;

	return Resource::Type();
}
