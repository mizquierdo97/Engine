#pragma once
#pragma once

#include "Resource.h"
#include "ModuleAssimp.h"
class ResourceMesh : public Resource {

public :
	ResourceMesh() {};
	ResourceMesh(UUID uid) {
		uuid = uid;
	};
	~ResourceMesh() {};
	bool LoadInMemory();


public:
	std::string path_name;
	Mesh obj_mesh;
	ComponentMaterial* mesh_material;
	Mesh bb_mesh;
	math::AABB box;
	int material_index = -1;


};