#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"

void ComponentMesh::UpdateComponent()
{

	if (App->renderer3D->render_fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		App->renderer3D->Render(this);
	}
	if (App->renderer3D->render_wireframe) {
		glColor3f(0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		App->renderer3D->Render(this);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	if (obj_mesh.norms != nullptr && App->gui->show_normals)
		App->physics->DrawNormals(this);
}
