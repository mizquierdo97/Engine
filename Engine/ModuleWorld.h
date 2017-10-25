#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "Texture.h"
#include "Object.h"
#include "Quadtree.h"

class Data;
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

	void SaveScene()const;
	void RecursiveSaveScene(std::vector<GameObject*>,Data*,int* i);
	void LoadScene();
	

	bool Options();//World Options

	void SetSelectedObject(GameObject* val) {
		selected_object = val;
	};
	GameObject* GetSelectedObject() {
		return selected_object;
	};
	bool IsObjectSelected() {
		return selected_object != nullptr;
	};

public:

	Texture* world_texture;
	std::vector<GameObject*> obj_vector;
	
	Quadtree quadtree;

private:
	GameObject* selected_object = nullptr;
};
