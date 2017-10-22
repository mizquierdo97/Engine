#pragma once
#include "ModuleAssimp.h"
#include "Texture.h"
#include "MathGeoLib.h"

enum objectType {
	null,
	cube,
	sphere,
	cylinder,
	aabb,
};

Mesh CreateCube();
Mesh CreateCylinder();
Mesh CreateAABB(AABB);
Mesh UpdateAABB(Mesh, AABB);
int CreateObject();

class Component;
class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;
class ComponentCamera;
class Object {

public:
	Object() {
		obj_name = new char[3];
		obj_name = "%i";
		obj_parent = nullptr;
	}
	Object(const Object& obj): obj_components(obj.obj_components), obj_parent(obj.obj_parent), obj_childs(obj.obj_childs) {
		

	}
	~Object() {

	}

	void AddComponentMesh(Mesh);
	void AddComponentTransform(aiVector3D, aiQuaternion, aiVector3D);
	void AddComponentTransform();
	void AddComponentMaterial(Texture* );
	void AddComponentCamera();
	void Update();


	ComponentMesh* GetMesh();
	ComponentMaterial* GetMaterial();
	ComponentTransform* GetTransform();
	ComponentCamera* GetCamera();
public:
	
	objectType obj_type = objectType::null;
	int obj_id = 0;	
	bool enabled;
	char* obj_name;


	//HIERARCHY
	std::vector<Object*> obj_childs;
	Object* obj_parent;
	std::vector<Component*> obj_components;
};