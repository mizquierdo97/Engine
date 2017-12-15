#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Resource.h"
#include <map>

class Application;

class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager(bool start_enabled = true);

	// Destructor
	virtual ~ModuleResourceManager() {
		
	};

	bool Init();

	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
public:
	UUID Find(const char* file_in_assets) const;
	UUID FindImported(const char* file_in_library) const;
	UUID ImportFile(const char* new_file_in_assets, bool force = false);
	UUID ImportShader(const char * path, Resource::Type type);
	UUID ImportMaterial(const char* path, ShaderProgram* program);
	UUID GenerateNewUID();
	const Resource* Get(UUID uid) const;
	Resource* Get(UUID uid);
	Resource* CreateNewResource(Resource::Type type, UUID force_uid = IID_NULL);

	Resource::Type TypeFromExtension(const char*);

public:
	std::map<UUID, Resource*> resources;
private:
	UUID last_uid = IID_NULL;

};
