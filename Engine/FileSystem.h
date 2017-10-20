#pragma once
#include "ModuleAssimp.h"
#include "MeshImporter.h"
struct Mesh;

class FileSystem {

public:
	FileSystem() {
		InitFileSystem();
		mesh_importer = new MeshImporter();
	}
	~FileSystem() {
		//delete mesh_importer;
	}

	void InitFileSystem();	
	const char* ImportImage(const char* name);

public:
	MeshImporter* mesh_importer; public:
	

};