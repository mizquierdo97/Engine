#ifndef _MODULEASSIMP
#define _MODULEASSIMP
#include "Module.h"
#include "Globals.h"
#include <vector>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "MathGeoLib.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct Mesh {

	uint id_buffer = 0;
	uint id_attributes = 0;
	
	uint id_vertexs = 0;
	uint num_vertexs = 0;
	float* vertexs = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_norms = 0;
	float* norms = nullptr;

	uint id_colors = 0;
	float* colors = nullptr;

	uint id_textures = 0;
	float* texture_coords = nullptr;


	
	float3 translation;
	float3 scale;
	Quat rotation;

	std::string mesh_path;

};


class ModuleAssimp : public Module
{
public:

	ModuleAssimp(bool start_enabled = true);
	~ModuleAssimp();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	
	bool CleanUp();

	bool ImportMesh(const char* path, std::string*);

public:



};

#endif