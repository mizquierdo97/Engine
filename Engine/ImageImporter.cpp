#include "ImageImporter.h"
#include <vector>
#include <Windows.h>
#include "Globals.h"
#include "Application.h"

bool ImageImporter::ImportImage(const char* path, std::string* file_path)
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
	std::string file_name = GetFileName(path);
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
				final_name += file_name.c_str(); final_name += ".dds";
				file_path[0] = final_name;
				if (!ExistsFile(final_name.c_str())) {
					pFile = fopen(final_name.c_str(), "wb");
					fwrite(data, sizeof(char), size, pFile);
					fclose(pFile);
				}

				if (std::find(App->gui->path_list.begin(), App->gui->path_list.end(), final_name) == App->gui->path_list.end())
					App->gui->path_list.push_back(final_name);
			}
			//ret = App->fs->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");

			RELEASE_ARRAY(data);
		}

		return true;
	}
	return false;
}

bool ImageImporter::loadTextureFromFile(const char* path, Texture** texture, bool is_texture)
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

	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT && is_texture)
	{
		iluFlipImage();
	}

	//Image loaded successfully
	if (success == IL_TRUE)
	{
		LOG("Image loaded succesfully");
		//Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE)
		{
			LOG("Image converted to RGBA");
			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT), texture);
			texture[0]->texture_path = path;
			//Create texture from file pixels
			textureLoaded = true;
		}

		//Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	//Report error
	if (!textureLoaded)
	{
		LOG("Can't load Image")
	}


	return textureLoaded;
}

bool ImageImporter::loadTextureFromPixels32(GLuint * pixels, GLuint width, GLuint height, Texture** texture)
{
	
	texture[0] = new Texture();

	texture[0]->Create(pixels, width, height);
	LOG("Texture Created");

	return true;
}

