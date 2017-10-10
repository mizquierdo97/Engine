#include "Application.h"
#include "ModuleWorld.h"

#include "imgui_dock.h"
#include <string>
ModuleWorld::ModuleWorld(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleWorld::~ModuleWorld()
{
}

bool ModuleWorld::Init()
{
	
	return true;
}

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128
bool ModuleWorld::Start() {
	
	//Create Screen Texture
	world_texture = new Texture();
	world_texture->Create(nullptr, App->window->width, App->window->height);

	
	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{
	//IF a files is dropped on the screen
	if (App->input->file_dropped) {
		FileDropped();
	}
	
	world_texture->Bind();
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float dt)
{
	//Iterate Object List and Render
	It_Render();
	//Render Imgui
	ImGui::Render();	

	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	delete world_texture;
	return true;
}

void ModuleWorld::FileDropped()
{
	std::string path = App->input->dropped_filedir;
	std::string temp;

	std::transform(path.begin(), path.end(), path.begin(), ::tolower);
	temp = path.substr(path.length() - 3, 3);


	if (!strcmp((char*)temp.c_str(), "fbx"))
		App->gui->path_list->push_back(path);

	else if (!strcmp((char*)temp.c_str(), "png"))
		App->gui->path_list->push_back(path);

	App->input->file_dropped = false;
}

void ModuleWorld::It_Render()
{
	//Render polygons
	if (App->renderer3D->render_fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		std::vector<Object*>::iterator temp = obj_vector.begin();
		while (temp != obj_vector.end()) {
			if ((*temp)->render_object)
				App->renderer3D->Render((*temp));

			temp++;
		}
	}

	//Render Wireframe
	if (App->renderer3D->render_wireframe) {
		glColor3f(0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::vector<Object*>::iterator temp = obj_vector.begin();
		while (temp != obj_vector.end()) {

			App->renderer3D->Render((*temp));
			if ((*temp)->obj_mesh.norms != nullptr && App->gui->show_normals)
				App->physics->DrawNormals((*temp));

			temp++;
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	world_texture->Unbind();
}

void ModuleWorld::ImportMesh(char * path)
{
	App->assimp->ImportMesh(path);
}


bool ModuleWorld::Options()
{
	if (ImGui::BeginDock("Scene", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)world_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
	}
	
	ImGui::EndDock();

	if (ImGui::BeginDock("Configuration", false, false, false,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_ShowBorders)) {

		std::vector<Object*>::iterator item = obj_vector.begin();
		int num = 0;
		while(item != obj_vector.end())		
		{
			if ((*item)->is_mesh) {
				ImGui::Separator();

				ImGui::Text("Mesh %i", num + 1);
				float3 t_temp = (*item)->obj_mesh.translation;

				ImGui::Text("Translation");
				ImGui::Text(" X : %f", t_temp.x);
				ImGui::SameLine();  ImGui::Text("Y : %f", t_temp.y);
				ImGui::SameLine();  ImGui::Text("Z : %f", t_temp.z);


				math::Quat q_temp = (*item)->obj_mesh.rotation;
				float3 eul_ang = q_temp.ToEulerXYZ();

				ImGui::Text("Rotation");

				ImGui::Text("X : %f", eul_ang.x);
				ImGui::SameLine();  ImGui::Text("Y : %f", eul_ang.y);
				ImGui::SameLine();  ImGui::Text("Z : %f", eul_ang.z);

				float3 s_temp = (*item)->obj_mesh.scale;

				ImGui::Text("Scale");
				ImGui::Text("X : %f", s_temp.x);
				ImGui::SameLine();  ImGui::Text("Y : %f", s_temp.y);
				ImGui::SameLine();  ImGui::Text("Z : %f", s_temp.z);
				num++;
			}
			item++;
		}
	}
	
	ImGui::EndDock();
	return true;
}