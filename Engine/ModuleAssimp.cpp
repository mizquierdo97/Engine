#include "Globals.h"
#include "Application.h"
#include "ModuleAssimp.h"
#include "FileSystem.h"
ModuleAssimp::ModuleAssimp(bool start_enabled) : Module(start_enabled)
{
}

ModuleAssimp::~ModuleAssimp()
{
}

bool ModuleAssimp::Init()
{	
	return true;
}

bool ModuleAssimp::Start()
{
	return true;
}

update_status ModuleAssimp::PreUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleAssimp::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAssimp::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleAssimp::CleanUp()
{
	
	return true;
}

bool ModuleAssimp::ImportMesh(const char * path,std::string* file_path)
{
	LOG("Start Exporting");	
	
	file_path[0] = "";

	const aiScene* scene;
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr) {
		std::string finalpath = path;
		std::string str_path = path;
		std::string str_temp;
		int length = 0;
		while (str_path.back() != '/')
		{
			str_path.pop_back();
			length++;
		}
		str_temp = finalpath.substr(finalpath.length() - length, length);
		while (str_temp.back() != '.')
		{
			str_temp.pop_back();
		}
		str_temp.pop_back();
		
		file_path[0] = MESHES_PATH; file_path[0] += str_temp; file_path[0] += ".bin";
		App->filesystem->mesh_importer->ImportFBX((aiScene*)scene, path, str_temp.c_str());
		aiReleaseImport(scene);

		return true;
	}
	return false;
}

