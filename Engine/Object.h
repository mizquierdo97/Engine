#pragma once
#include "ModuleAssimp.h"
#include "Texture.h"
#include "MathGeoLib.h"

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
int CreateObject();
void CreateObject(GameObject*);

class Component;
class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;
class ComponentCamera;


class GameObject {

public:
	GameObject() {
		obj_name = new char[3];
		//obj_name = "%i";
		obj_parent = nullptr;
		local_bbox.SetNegativeInfinity();
	}
	GameObject(const GameObject& obj): obj_components(obj.obj_components), obj_parent(obj.obj_parent), obj_childs(obj.obj_childs) {
		

	}
	~GameObject() {

	}

	void AddComponentMesh(Mesh);
	void AddComponentTransform(aiVector3D, aiQuaternion, aiVector3D);
	void AddComponentTransform(float3, float4, float3);
	void AddComponentTransform();
	void AddComponentMaterial(Texture* );
	void AddComponentCamera();
	void Update();


	ComponentMesh* GetMesh();
	ComponentMaterial* GetMaterial();
	ComponentTransform* GetTransform();
	ComponentCamera* GetCamera();

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
	void SetStatic(bool val) {
		is_static = val;
	};


	bool IsEnabled() {
		return enabled;
	};
	void SetEnable(bool val) {
		enabled = val;
	};


	std::string GetName() {
		return obj_name;
	};

	void SetName(std::string name) {
		obj_name = name;
		
	};

private:
	
	objectType obj_type = objectType::null;
	int obj_id = 0;	
	bool enabled = true;
	std::string obj_name = "";
	bool is_static = false;


	AABB local_bbox;
	AABB global_bbox;

public:
	//HIERARCHY
	UUID obj_uuid;
	UUID parent_uuid;
	std::vector<GameObject*> obj_childs;
	GameObject* obj_parent;
	std::vector<Component*> obj_components;
};