#include "Application.h"
#include "ModuleResourceManager.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ResourceShader.h"
#include "ResourceShaderProgram.h"
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

	for (std::map<UUID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		RELEASE((*it).second);

	resources.clear();

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
		
		std::string file = NormalizePath(file_in_assets).c_str();
		
		for (std::map<UUID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->first!=IID_NULL && it->second->file.compare(file) == 0)
				return it->first;
		}
		return IID_NULL;
}

UUID ModuleResourceManager::FindImported(const char * file_in_library) const
{
	std::string file(file_in_library);

	for (std::map<UUID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->first == IID_NULL) continue;
		if (it->second->exported_file.compare(file) == 0)
			return it->first;
	}
	return IID_NULL;
}

UUID ModuleResourceManager::ImportFile(const char * path, bool force)
{
	UUID null_uuid; UuidCreateNil(&null_uuid);
	NormalizePath((char*)path);
	UUID obj_uuid = Find(path);	
	RPC_STATUS stat;

	if (UuidCompare(&obj_uuid, &null_uuid,&stat)==0 || force) {
		Resource::Type type = TypeFromExtension(path);
		std::string file_path;
		bool import_ok = false;
		switch (type) {
		case Resource::texture:
			import_ok = App->filesystem->image_importer->ImportImage(path, &file_path,force);			
			break;
		
		case Resource::mesh:
			import_ok = App->assimp->ImportMesh(path, &file_path);
			break;
		
	}

		if (import_ok && strcmp(file_path.c_str(), "")) {

			Resource* res = CreateNewResource(type);
			res->file = path;
			res->exported_file = file_path;
			res->type = type;
			obj_uuid = res->uuid;
		}
	}
	
	return obj_uuid;
}

UUID ModuleResourceManager::ImportShader(const char * path, Resource::Type type)
{

	Shader* new_shader = new Shader();

	ShaderType s_type;

	switch (type) {
	case Resource::fragment_shader:
		s_type = ShaderType::pixel_shader;
		break;
	case Resource::vertex_shader:
		s_type = ShaderType::vertex_shader;
		break;
	}
	new_shader->shader_id = App->filesystem->shader_importer->loadShaderFromFile(path, s_type);

	UUID obj_uuid = Find(path);
	UUID null_uuid; UuidCreateNil(&null_uuid);
	RPC_STATUS stat;

	//TODO probably dont need
	if (UuidCompare(&obj_uuid, &null_uuid, &stat) == 0) {
		Resource* res = CreateNewResource(type);
		res->file = path;
		res->exported_file = path;
		res->type = type;
		obj_uuid = res->uuid;

		((ResourceShader*)res)->res_shader = new_shader;	
		
	}
	

	return obj_uuid;
}

UUID ModuleResourceManager::ImportMaterial(const char * path , ShaderProgram* program)
{
	
	UUID obj_uuid = Find(path);
	UUID null_uuid; UuidCreateNil(&null_uuid);
	RPC_STATUS stat;

	//TODO probably dont need
	if (UuidCompare(&obj_uuid, &null_uuid, &stat) == 0) {
		ResourceShaderProgram* res = (ResourceShaderProgram*)CreateNewResource(Resource::shader_program);
		
		res->file = path;
		res->exported_file = path;
		res->type = Resource::shader_program;
		res->res_shader_program = program;
		obj_uuid = res->uuid;
	}

	return obj_uuid;
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
	case Resource::mesh:
		ret = (Resource*) new ResourceMesh(uuid);
		break;
	case Resource::fragment_shader:
		ret = (Resource*) new ResourceShader(uuid);
		break;
	case Resource::vertex_shader:
		ret = (Resource*) new ResourceShader(uuid);
		break;
	case Resource::shader_program:
		ret = (Resource*) new ResourceShaderProgram(uuid);
		break;
	}

	
	resources[uuid] = ret;	

	return ret;
}

Resource::Type ModuleResourceManager::TypeFromExtension(const char * path)
{
	std::string extension = GetExtension(path).c_str();

	if (!strcmp(extension.c_str(), "png"))
		return Resource::texture;
	if (!strcmp(extension.c_str(), "tga"))
		return Resource::texture;
	if (!strcmp(extension.c_str(), "jpg"))
		return Resource::texture;
	if (!strcmp(extension.c_str(), "fbx"))
		return Resource::mesh;
	if (!strcmp(extension.c_str(), "frsh"))
		return Resource::fragment_shader;
	if (!strcmp(extension.c_str(), "vrsh"))
		return Resource::vertex_shader;

	return Resource::Type();
}
