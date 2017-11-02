#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Object.h"

void ComponentMesh::UpdateComponent()
{
	
	AABB transformed_bounding_box = UpdateAABB(GetParent());
	
	ComponentCamera* active_camera = App->renderer3D->GetActiveCamera();
	if (active_camera->cam_frustum.ContainsAaBox(transformed_bounding_box) || !active_camera->frustum_culling) {
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
}

void ComponentMesh::SaveComponentScene(Data* data)
{
	data->CreateSection("Mesh");
	data->AddInt("Num Vertexs", obj_mesh.num_vertexs);
	data->AddInt("Num Indices", obj_mesh.num_indices);
	data->AddBool("Norms", obj_mesh.id_norms);
	data->AddBool("Texture", obj_mesh.id_textures);
	data->AddString("Mesh Path", path_name);

	data->CloseSection();
}
