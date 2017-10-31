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

void ModuleAssimp::ImportMesh(const char * path)
{
	

	LOG("Start Exporting");
	
	
	const aiScene* scene;
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
		
			std::string finalpath = path;
			std::string str_path = path;
			std::string str_temp;
			int length=0;
			while (str_path.back() != '\\')
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
			
			
			App->filesystem->mesh_importer->ImportMesh((aiScene*)scene, MESHES_PATH, str_temp.c_str() );
			aiReleaseImport(scene);
		
			
}

			//RELEASE SCENE



