#ifndef _FILESYSTEM
#define _FILESYSTEM

#include "ModuleAssimp.h"
#include "MeshImporter.h"
#include "ImageImporter.h"
#include "ShaderImporter.h"
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
	void LoadByExtension(std::string);

public:
	MeshImporter* mesh_importer;
	ImageImporter* image_importer;
	ShaderImporter* shader_importer;

};
#endif