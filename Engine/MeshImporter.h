#pragma once

class MeshImporter {
public:
	MeshImporter() {

	}
	~MeshImporter() {

	}

	void ImportMesh(aiScene* m, const char* directory, const char* name);
	void LoadMesh(const char*);
	Mesh LoadComponentMesh(char* name, uint*);

};