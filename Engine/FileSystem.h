#pragma once
#include "ModuleAssimp.h"

struct Mesh;
class FileSystem {
	
public:
	FileSystem() {
		InitFileSystem();
	}
	~FileSystem() {

	}

	void InitFileSystem();
	void ImportMesh(aiMesh* m, const char* directory ,const char* name);
	void LoadMesh(const char*);
};