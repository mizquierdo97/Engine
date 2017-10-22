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

		AABB temp;
		temp.SetFrom((vec*)m.vertexs, m.num_vertexs);
		obj_mesh.bounding_box = temp;
		bb_mesh = CreateAABB(temp);
		

	}
	~ComponentMesh() {};

	void UpdateComponent();
public:
	Mesh obj_mesh;
	ComponentMaterial* mesh_material;
	Mesh bb_mesh;
};