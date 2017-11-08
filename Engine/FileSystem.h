#pragma once
#include "ModuleAssimp.h"
#include "MeshImporter.h"
struct Mesh;

class FileSystem {

public:
	FileSystem() {
		mesh_importer = new MeshImporter();
	}
	~FileSystem() {
		RELEASE(mesh_importer);
	}

	void InitFileSystem();	
	bool ImportImage(const char* name, std::string*);

public:
	MeshImporter* mesh_importer; public:
	

};