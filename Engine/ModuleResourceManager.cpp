#include "Application.h"
#include "ModuleResourceManager.h"
#include "Resource.h"
#include "ResourceTexture.h"
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

UUID ModuleResourceManager::Find(const char * file_in_assets) const
{	
		std::string file(file_in_assets);
		
		for (std::map<UUID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->second->file.compare(file) == 0)
				return it->first;
		}
		return IID_NULL;
}

UUID ModuleResourceManager::FindImported(const char * file_in_library) const
{
	std::string file(file_in_library);

	for (std::map<UUID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->exported_file.compare(file) == 0)
			return it->first;
	}
	return IID_NULL;
}

UUID ModuleResourceManager::ImportFile(const char * path, bool force)
{
	UUID null_uuid; UuidCreateNil(&null_uuid);
	UUID obj_uuid = Find(path);	
	RPC_STATUS stat;

	if (UuidCompare(&obj_uuid, &null_uuid,&stat)==0) {
		Resource::Type type = TypeFromExtension(path);
		std::string file_path;
		bool import_ok = false;
		switch (type) {
		case Resource::texture:
			import_ok = App->filesystem->ImportImage(path, &file_path);
			break;
		}

		if (import_ok && strcmp(file_path.c_str(), "")) {

			Resource* res = CreateNewResource(type);
			res->file = path;
			res->exported_file = file_path;
			res->type = type;
		}
	}
	else {
		Resource* res = Get(obj_uuid);
		int x = 0;
	}
	return UUID();
}
bool operator>(UUID l,  UUID r) {
	RPC_STATUS stat;
	int i = UuidCompare(&l, &r,&stat);

	return i > 0;
}

bool operator<(UUID l, UUID r) {
	RPC_STATUS stat;
	int i = UuidCompare(&l, &r, &stat);

	return i < 0;
}
Resource * ModuleResourceManager::Get(UUID uid)
{
	return resources[uid];

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
		ret = (Resource*) new ResourceTexture(uuid);
		break;

	}
	
	resources[uuid] = ret;	

	return ret;
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
