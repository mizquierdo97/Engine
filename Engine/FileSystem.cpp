#include "FileSystem.h"
#include "Application.h"
#include "ModuleAssimp.h"
#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;
void FileSystem::InitFileSystem()
{
	if (CreateDirectory(ASSETS_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		
	}
	if (CreateDirectory(LIBRARY_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	if (CreateDirectory(MESHES_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	if (CreateDirectory(MATERIALS_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}


	std::string path = ASSETS_PATH;
	for (auto & p : fs::directory_iterator(path)) {	

		//GET STRING
		const wchar_t* temp = p.path().c_str();
		std::wstring ws(temp);
		std::string str(ws.begin(), ws.end());
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		//

		//GET EXTENSION

		std::string file_extension = GetExtension(str);
		
		//

		//SWITCH
		if (!strcmp(file_extension.c_str(), "fbx")) {
			App->assimp->ImportMesh(str.c_str());
		}
		if (!strcmp(file_extension.c_str(), "png") || !strcmp(file_extension.c_str(), "jpg") || !strcmp(file_extension.c_str(), "tga")) {
			App->resources->ImportFile(str.c_str());
			//App->filesystem->ImportImage(str.c_str());
		}

	}
}

