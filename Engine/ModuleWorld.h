#ifndef _MODULEWORLD
#define _MODULEWORLD

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "Texture.h"
#include "Object.h"
#include "Quadtree.h"
#include "ImGui\ImGuizmo.h"

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


	int HierarchyRecurs(std::vector<GameObject*> item, int*, int, int, bool*);
	void FileDropped();
	void It_Render(); //Iterate Render
	void It_Update();
	void DebugDraw();

	void SaveScene(const char*)const;
	void RecursiveSaveScene(std::vector<GameObject*>, Data*, int* i);
	void LoadScene(const char*);
	void RedistributeGameObjects();
	void RecursiveCreateAABB(GameObject*);
	void DeleteObject(std::vector<GameObject*>);

	void LoadSceneGoData(Data, GameObject*);
	void LoadSceneMesh(Data, GameObject*);
	void LoadSceneMaterial(Data, GameObject*);
	void LoadSceneCamera(Data, GameObject*);
	void ClearScene();

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

	bool ImFocused()const {
		return world_focus;
	}
	void Focus(bool val) {
		world_focus = val;
	}

public:

	//BUTTONS TEXTURES
	Texture*	play_tex;
	Texture*	pause_tex;
	Texture*	nframe_tex;
	Texture*	stop_tex;

	//WORLD TEXTURE
	Texture*	world_texture;
	float4		world_tex_vec;

	Quadtree	quadtree;
	bool		using_octree = true;
	bool		see_octree = true;

	std::vector<GameObject*>					obj_vector;
	std::vector<std::pair<GameObject*, UUID>>	uuid_vect;

	std::list<GameObject*>	static_list;
	std::list<GameObject*>	non_static_list;

	ImGuizmo::OPERATION Operation = ImGuizmo::TRANSLATE;

private:
	GameObject*		selected_object = nullptr;
	bool			world_focus = false;
	



};


#endif