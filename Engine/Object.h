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
void CreateAABB(AABB);
int CreateObject(objectType type);

class Component;
class Object {

public:
	Object() {

	}
	~Object() {

	}

	void AddComponent();
	void Update();

public:
	Mesh obj_mesh;
	Texture* obj_text = nullptr;
	objectType obj_type = objectType::null;
	int obj_id = 0;
	bool render_object = true;
	bool is_mesh = true;

	bool enabled;
	char* obj_name;
	//char* obj_tag;

	//HIERARCHY
	std::vector<Object*> obj_childs;
	Object* obj_parent;
	std::vector<Component*> obj_components;
};