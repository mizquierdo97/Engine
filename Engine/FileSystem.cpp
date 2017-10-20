#include "FileSystem.h"
#include <vector>
#include <Windows.h>
#include "Globals.h"
#include "ModuleAssimp.h"
#include "Application.h"


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
}


const char* FileSystem::ImportImage(const char* path)
{
	//Texture loading success
	bool textureLoaded = false;
	LOG("Loading Texture: %s", path);
	//Generate and set current image ID
	uint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
		//Load image
	ILboolean success = ilLoadImage(path);
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	std::string final_name;
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}
	//Image loaded successfully
	if (success == IL_TRUE)
	{
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) { // Save to buffer with the ilSaveIL function
				FILE * pFile;	
			
				final_name = MESHES_PATH;
				final_name += "image"; final_name += ".dds";
				pFile = fopen(final_name.c_str(), "wb");
				fwrite(data, sizeof(char), size, pFile);
				fclose(pFile);
				App->gui->path_list->push_back(final_name);
			}
				//ret = App->fs->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");

			delete[] data;
		}


	}
	return final_name.c_str();
}

//OK
