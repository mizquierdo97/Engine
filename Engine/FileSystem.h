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
	const char* ImportImage(const char* name);

public:
	MeshImporter* mesh_importer; public:
	

};