#pragma once
#include "Component.h"
#include "ModuleAssimp.h"
#include <string>
#include "ResourceMesh.h"


class ComponentMesh : public Component {
public:

	ComponentMesh() {};
	ComponentMesh(UUID uuid,GameObject* obj)  {
		res_uuid = uuid;
		SetParent(obj);
		
		comp_type = ComponentType::mesh;

		ResourceMesh* res = (ResourceMesh*)GetResource();

		
		AABB temp;
		temp.SetFrom((float3*)res->obj_mesh.vertexs, res->obj_mesh.num_vertexs);
		GetParent()->SetLocalBox(temp);		
		
		path_name = res->obj_mesh.mesh_path;

	}
	~ComponentMesh() {

		
	};

	void UpdateComponent();
	void SaveComponentScene(Data* data);
	void ShowInspectorComponents();
public:
	std::string path_name;
	//Mesh obj_mesh;

	math::AABB box;
};