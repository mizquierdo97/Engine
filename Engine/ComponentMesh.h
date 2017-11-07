#pragma once
#include "Component.h"
#include "ModuleAssimp.h"
#include <string>
class ComponentMesh : public Component {
public:

	ComponentMesh() {};
	ComponentMesh(Mesh m,GameObject* obj) : obj_mesh(m)  {
		SetParent(obj);
		mesh_material = nullptr;
		comp_type = ComponentType::mesh;

		AABB temp;
		temp.SetFrom((float3*)m.vertexs, m.num_vertexs);
		GetParent()->SetLocalBox(temp);
		bb_mesh = CreateAABB(temp);
		
		path_name = m.mesh_path;

	}
	~ComponentMesh() {
		RELEASE_ARRAY(bb_mesh.vertexs);
	};

	void UpdateComponent();
	void SaveComponentScene(Data* data);
public:
	std::string path_name;
	Mesh obj_mesh;
	ComponentMaterial* mesh_material;
	Mesh bb_mesh;
	math::AABB box;
};