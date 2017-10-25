#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "Texture.h"
#include "Object.h"
#include "Quadtree.h"


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
	int HierarchyRecurs(std::vector<GameObject*> item,int*, int,int);
	void FileDropped();
	void It_Render(); //Iterate Render
	void DebugDraw();

	bool Options();//World Options
public:

	Texture* world_texture;

	std::vector<GameObject*> obj_vector;
	GameObject* selected_object = nullptr;

	Quadtree quadtree;
};
