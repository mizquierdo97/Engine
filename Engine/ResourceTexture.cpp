#include "ResourceTexture.h"
#include "Application.h"
bool ResourceTexture::LoadInMemory()
{
	return App->renderer3D->loadTextureFromFile(exported_file.c_str(),&res_tex);
}