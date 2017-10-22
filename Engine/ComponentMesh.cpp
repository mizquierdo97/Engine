#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Object.h"

void ComponentMesh::UpdateComponent()
{

	float4x4 matrix = parent->GetTransform()->GetMatrix();
	float4 new_min_point = matrix * float4(obj_mesh.bounding_box.minPoint, 1);
	float4 new_max_point = matrix * float4(obj_mesh.bounding_box.maxPoint, 1);

	AABB transformed_bounding_box = obj_mesh.bounding_box;
	transformed_bounding_box.minPoint = { new_min_point.x, new_min_point.y, new_min_point.z };
	transformed_bounding_box.maxPoint = { new_max_point.x, new_max_point.y, new_max_point.z };

	bb_mesh = UpdateAABB(bb_mesh, transformed_bounding_box);

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
