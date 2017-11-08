#include "ResourceTexture.h"
#include "Application.h"
bool ResourceTexture::LoadInMemory()
{
	return App->filesystem->image_importer->loadTextureFromFile(exported_file.c_str(),&res_tex);
}