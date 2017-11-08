#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleAssimp.h"
bool ResourceMesh::LoadInMemory()
{
	return App->filesystem->mesh_importer->CreateMesh(exported_file.c_str(),this);

}