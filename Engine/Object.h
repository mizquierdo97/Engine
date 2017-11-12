#pragma once
#include "ModuleAssimp.h"
#include "Texture.h"
#include "MathGeoLib.h"
#include "Component.h"
#include "ComponentTransform.h"
#pragma comment(lib, "rpcrt4.lib")

enum objectType {
	null,
	cube,
	sphere,
	cylinder,
	aabb,
};

class GameObject;
Mesh CreateCube();
Mesh CreateCylinder();
Mesh CreateAABB(AABB);
Mesh UpdateAABB(Mesh, AABB);
AABB UpdateAABB(GameObject*);
int CreateObject();
void CreateObject(GameObject*, UUID force = IID_NULL);

class Component;
class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;
class ComponentCamera;


class GameObject {

public:
	GameObject() {
		
		//obj_name = "%i";
		obj_parent = nullptr;
		local_bbox.SetNegativeInfinity();
	}
	GameObject(const GameObject& obj): obj_components(obj.obj_components), obj_parent(obj.obj_parent), obj_childs(obj.obj_childs) {
		

	}
	~GameObject() {

		if (obj_childs.size()) {
			std::vector<GameObject*>::iterator item = obj_childs.begin();
			while (item != obj_childs.end()) {
				RELEASE((*item));
				item++;
			}
			obj_childs.clear();
		}
		

		if (obj_components.size()) {
			std::vector<Component*>::iterator item_comp = obj_components.begin();
			while (item_comp != obj_components.end()) {
				RELEASE((*item_comp));
				item_comp++;
			}
			obj_components.clear();
		}


	}

	void AddComponentMesh(UUID);
	void AddComponentTransform(aiVector3D, aiQuaternion, aiVector3D);
	void AddComponentTransform(float3, float4, float3);
	void AddComponentTransform();
	void AddComponentMaterial(UUID );
	void AddComponentCamera();
	void AddComponentCamera(Frustum);
	void Update();
	void Draw();

	ComponentMesh* GetMesh();
	ComponentMaterial* GetMaterial();
	ComponentTransform* GetTransform();
	ComponentCamera* GetCamera();

	Component* FindComponentbytype(ComponentType type)const;

	AABB GetLocalBBox() {
		return local_bbox;
	};
	void SetLocalBox(AABB box) {
		local_bbox = box;
	}

	AABB GetGlobalBBox() {
		return global_bbox;
	};
	void SetGlobalBox(AABB box) {
		global_bbox = box;
	}


	bool IsStatic() {
		return is_static;
	};

	void SetStatic(bool val);

	bool IsEnabled() {
		return enabled;
	};
	void SetEnable(bool val) {
		enabled = val;
	};


	std::string GetName() {
		return obj_name;
	}
	void SetName(std::string name) {
		obj_name = name;
	};

private:
	
	objectType obj_type = objectType::null;
	
	int obj_id = 0;	
	bool enabled = true;
	
	bool is_static = false;
	std::string obj_name = "";

	AABB local_bbox;
	AABB global_bbox;

public:
	//HIERARCHY
	UUID obj_uuid;
	UUID parent_uuid;
	GameObject* obj_parent = nullptr;
	std::vector<Component*> obj_components;
	std::vector<GameObject*> obj_childs;
};