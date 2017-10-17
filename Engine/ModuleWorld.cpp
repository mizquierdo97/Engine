#include "Application.h"
#include "ModuleWorld.h"

#include "imgui_dock.h"
#include "Object.h"
#include "ComponentMesh.h"
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
		App->assimp->ImportMesh((char*)path.c_str());

	else if (!strcmp((char*)temp.c_str(), "png") || !strcmp((char*)temp.c_str(), "jpg"))
		App->gui->path_list->push_back(path);

	App->input->file_dropped = false;
}

void ModuleWorld::It_Render()
{
	//Render polygons


	//RENDER MESHES
	
	std::vector<Object*>::iterator temp = obj_vector.begin();
	while (temp != obj_vector.end()) {
		(*temp)->Update();
		temp++;
	}


/*	if (App->renderer3D->render_fill) {
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
	}*/
	glColor3f(1.0f, 1.0f, 1.0f);
	world_texture->Unbind();
}

void ModuleWorld::ImportMesh(char * path)
{
	App->filesystem->LoadMesh(path);
	//App->assimp->ImportMesh(path);
}


bool ModuleWorld::Options()
{
	if (ImGui::BeginDock("Scene", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)world_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
	}
	
	ImGui::EndDock();

	if (ImGui::BeginDock("Configuration", false, false, false)) {

		std::vector<Object*>::iterator item = obj_vector.begin();
		int num = 0;
		while(item != obj_vector.end())		
		{
			if ((*item)->is_mesh) {
				ImGui::Separator();
				Mesh m = (*item)->GetMesh()->obj_mesh;
				ImGui::Text("Mesh %i", num + 1);
				float3 t_temp = m.translation;

				ImGui::Text("Translation");
				ImGui::Text("X :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", t_temp.x); ImGui::SameLine();
				ImGui::Text("Y :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", t_temp.y); ImGui::SameLine();
				ImGui::Text("Z :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", t_temp.z); 

				math::Quat q_temp = m.rotation;
				float3 eul_ang = q_temp.ToEulerXYZ();

				ImGui::Text("Rotation");

				ImGui::Text("X :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", eul_ang.x); ImGui::SameLine();
				ImGui::Text("Y :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", eul_ang.y); ImGui::SameLine();
				ImGui::Text("Z :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", eul_ang.z); 

				float3 s_temp = m.scale;

				ImGui::Text("Scale");
				ImGui::Text("X :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", s_temp.x); ImGui::SameLine();
				ImGui::Text("Y :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", s_temp.y); ImGui::SameLine();
				ImGui::Text("Z :"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", s_temp.z); 
				

				ImGui::Text("Tris:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%i", m.num_vertexs/3);
				ImGui::SameLine();
				ImGui::Text("Vertexs:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%i", m.num_vertexs);

				num++;
			}
			item++;
		}
	}
	
	ImGui::EndDock();

	if(ImGui::BeginDock("Hierarchy", false, false, false)) {

		ImGui::EndDock();
	}
	return true;
}