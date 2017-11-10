#pragma once

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
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
	void It_Update();
	void DebugDraw();

	void SaveScene(const char* )const;
	void RecursiveSaveScene(std::vector<GameObject*>,Data*,int* i);
	void LoadScene(const char* );
	void RedistributeGameObjects();
	void RecursiveCreateAABB(GameObject*);

	void LoadSceneGoData(Data, GameObject*);
	void LoadSceneMesh(Data,GameObject*);
	void LoadSceneMaterial(Data, GameObject*);
	
	GameObject * Raycast(const LineSegment & segment, float  &dist)const;	
	void Recursivetest(const LineSegment& segment, float& dist, GameObject** closest_object)const ;

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
	Texture* play_tex;
	Texture* pause_tex;
	Texture* nframe_tex;
	Texture* stop_tex;

	Texture* world_texture;
	float4 world_tex_vec;
	std::vector<GameObject*> obj_vector;	
	Quadtree quadtree;
	std::vector<std::pair<GameObject*, UUID>> uuid_vect;
	bool using_octree = true;
	bool see_octree = true;
private:
	GameObject* selected_object = nullptr;

	
	
};
