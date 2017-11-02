#pragma once
#include <map>
#include "Glew/include/glew.h" 

class MeshImporter {
public:
	MeshImporter() {

	}
	~MeshImporter() {

	}

	void ImportMesh(aiScene* m, const char* directory, const char* name);
	void LoadMesh(const char*);
	Mesh LoadComponentMesh(char* name, uint*);
	int LoadGLTextures(const aiScene* scene);


public:
	std::map<GLuint, std::string> textureIdMap;
};