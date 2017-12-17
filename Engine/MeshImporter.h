#ifndef _MESHIMPORTER
#define _MESHIMPORTER
#include <map>
#include "Glew/include/glew.h" 

class ResourceMesh;
class MeshImporter {
public:
	MeshImporter() {

	}
	~MeshImporter() {

	}

	void ImportFBX(aiScene* m, const char* directory, const char* name);
	void LoadMesh(const char*);
	Mesh LoadComponentMesh(char* name, uint*);
	
	bool CreateMesh(const char* path,ResourceMesh*);
	void ImportMesh();


public:

	uint actual_ranges[5] = {0,0,0,0,0};
};
#endif