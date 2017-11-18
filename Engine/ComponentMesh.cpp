#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Object.h"

void ComponentMesh::UpdateComponent()
{
	//IMPROVE THIS, dont need to update every frame
	AABB transformed_bounding_box = this->GetParent()->GetGlobalBBox();
	if (!this->GetParent()->IsStatic())
	transformed_bounding_box = UpdateAABB(GetParent());	

	// DELETE THIS
	if (!App->world->using_octree) {
		ComponentCamera* active_camera = App->renderer3D->GetActiveCamera();
	if (active_camera->cam_frustum.ContainsAaBox(transformed_bounding_box) != -1 || !active_camera->frustum_culling) {
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
			if (((ResourceMesh*)GetResource())->obj_mesh.norms != nullptr && App->gui->show_normals)
				App->physics->DrawNormals(this);

		}
	}
}

void ComponentMesh::SaveComponentScene(Data* data)
{
	ResourceMesh* res = ((ResourceMesh*)GetResource());
	Mesh m = res->obj_mesh;
	data->CreateSection("Mesh");
	data->AddInt("Num Vertexs", m.num_vertexs);
	data->AddInt("Num Indices", m.num_indices);
	data->AddBool("Norms", m.id_norms);
	data->AddBool("Texture", m.id_textures);
	data->AddString("Mesh Path", res->exported_file);

	data->CloseSection();
}

void ComponentMesh::ShowInspectorComponents()
{
	ResourceMesh* res = ((ResourceMesh*)GetResource());
	if (ImGui::CollapsingHeader("Mesh")) {
		ImGui::Text("Original File: "); ImGui::SameLine();
		ImGui::Text(res->file.c_str());

		ImGui::Text("Imported File: "); ImGui::SameLine();
		ImGui::Text(res->exported_file.c_str());

		static bool b_open = false;
	
		if (ImGui::Button("Change Mesh")) {
				b_open = true;
		}
		
		if (b_open) {
			ImGui::SetNextWindowPos(ImVec2(App->input->GetMousepositionX() - 100, App->input->GetMousepositionY()), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
			ImGui::Begin("Meshes", &b_open);
			App->gui->ShowMeshMenu(this);
			ImGui::End();
		}
	
	}
}
