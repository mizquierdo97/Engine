#pragma once
#include "ModuleAssimp.h"
#include "Texture.h"

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

class Object {

public:
	Object() {

	}
	
public:
	Mesh obj_mesh;
	Texture* obj_text = nullptr;
	objectType obj_type = objectType::null;
	int obj_id = 0;
};