#include "ResourceTexture.h"
#include "Application.h"
bool ResourceTexture::LoadInMemory()
{
	return App->filesystem->image_importer->loadTextureFromFile(exported_file.c_str(),&res_tex);
}

bool ResourceTexture::EraseInMemory()
{
	RELEASE(res_tex);
	return false;
}

bool ResourceTexture::UpdateRes()
{
	App->filesystem->image_importer->UpdateTextureFromFile(res_tex->texture_path.c_str(),&res_tex);
	return true;
}
