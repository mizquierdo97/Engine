#pragma once
#include "ModuleAssimp.h"
#include "MeshImporter.h"
#include "ImageImporter.h"
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
	

public:
	MeshImporter* mesh_importer;
	ImageImporter* image_importer;

};