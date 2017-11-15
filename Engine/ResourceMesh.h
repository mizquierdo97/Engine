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
	~ResourceMesh() {

	};
	bool LoadInMemory();
	bool EraseInMemory() {
		return false;
	};
	bool UpdateRes() {
		return false;
	}

public:
	std::string path_name;
	Mesh obj_mesh;
	
	Mesh bb_mesh;
	math::AABB box;
	int material_index = -1;


};