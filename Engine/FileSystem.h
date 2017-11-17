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
	std::string CreateMeta(std::string);
	std::string CreateMetaMesh(std::string, std::string);
	void ImportFile(std::string, bool force = false);
	void CheckFilesUpdates();
	void UpdateMeta(std::string, std::string new_file_name, const char* name_data);

public:
	MeshImporter* mesh_importer;
	ImageImporter* image_importer;

};