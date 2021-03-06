#ifndef _RESOURCE_MESH
#define _RESOURCE_MESH

#include "Resource.h"
#include "ModuleAssimp.h"
class ResourceMesh : public Resource {

public :
	ResourceMesh() {};
	ResourceMesh(UUID uid) {
		uuid = uid;
	};
	~ResourceMesh() {
		if (obj_mesh.id_vertexs)
			RELEASE_ARRAY(obj_mesh.vertexs);
		if (obj_mesh.id_indices)
			RELEASE_ARRAY(obj_mesh.indices);
		if (obj_mesh.id_norms)
			RELEASE_ARRAY(obj_mesh.norms);
		if (obj_mesh.id_colors)
			RELEASE_ARRAY(obj_mesh.colors);
		if (obj_mesh.id_textures)
			RELEASE_ARRAY(obj_mesh.texture_coords);

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

	math::AABB box;
	int material_index = -1;


};

#endif