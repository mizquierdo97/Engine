#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Object.h"

void ComponentMesh::UpdateComponent()
{

	float4x4 matrix = GetParent()->GetTransform()->GetMatrix();
	float4 new_min_point = matrix * float4(GetParent()->GetLocalBBox().minPoint, 1);
	float4 new_max_point = matrix * float4(GetParent()->GetLocalBBox().maxPoint, 1);

	AABB transformed_bounding_box = GetParent()->GetLocalBBox();
	transformed_bounding_box.minPoint = { new_min_point.x, new_min_point.y, new_min_point.z };
	transformed_bounding_box.maxPoint = { new_max_point.x, new_max_point.y, new_max_point.z };

	bb_mesh = UpdateAABB(bb_mesh, transformed_bounding_box);
	GetParent()->SetGlobalBox(transformed_bounding_box);
	ComponentCamera* active_camera = App->renderer3D->GetActiveCamera();
	if (active_camera->cam_frustum.ContainsAaBox(transformed_bounding_box)) {
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
