#pragma once
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
	int LoadGLTextures(const aiScene* scene);
	bool CreateMesh(const char* path,ResourceMesh*);
	void ImportMesh();


public:
	std::map<GLuint, std::string> textureIdMap;

	uint actual_ranges[4] = {0,0,0,0};
};