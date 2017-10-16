#pragma once
#include "Component.h"
#include "ModuleAssimp.h"
class ComponentMesh : public Component {
public:

	ComponentMesh() {};
	ComponentMesh(Mesh m,Object* obj) : obj_mesh(m)  {
		parent = obj;
		mesh_material = nullptr;
		comp_type = ComponentType::mesh;
	}
	~ComponentMesh() {};

	void UpdateComponent();
public:
	Mesh obj_mesh;
	ComponentMaterial* mesh_material;
};