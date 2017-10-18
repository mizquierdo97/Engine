#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "Texture.h"
#include "Object.h"
class ModuleWorld : public Module
{
public: 

	ModuleWorld(bool start_enabled = true);
	~ModuleWorld();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	int HierarchyRecurs(Object* item);
	void FileDropped();
	void It_Render(); //Iterate Render
	void ImportMesh(char* path); //Import meshes
	bool Options();//World Options
public:

	Texture* world_texture;

	std::vector<Object*> obj_vector;
};
