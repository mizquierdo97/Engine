#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"

void ComponentMesh::UpdateComponent()
{
	App->renderer3D->Render(this);

}
