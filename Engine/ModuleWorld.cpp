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
	
	//App->renderer3D->loadTextureFromFile("Baker_house2.png");


	world_texture = new Texture();
	world_texture->Create(nullptr, App->window->width, App->window->height);

	
	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{
	if (App->input->file_dropped) {
		
		std::string path = App->input->dropped_filedir;
		std::string temp;
		temp = path.substr(path.length()-3, 3);
		
		if (!strcmp((char*)temp.c_str(), "fbx"))
			ImportMesh(App->input->dropped_filedir);
		else if(!strcmp((char*)temp.c_str(), "png"))
			App->renderer3D->loadTextureFromFile(App->input->dropped_filedir);
		App->input->file_dropped = false;
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

	//RENDER MESHES
	if (App->renderer3D->render_fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		std::vector<Object*>::iterator temp = obj_vector.begin();
		while (temp != obj_vector.end()) {

			App->renderer3D->Render((*temp));
			if ((*temp)->obj_mesh.norms != nullptr)
				App->physics->DrawNormals((*temp));
			temp++;
		}
	}
	if (App->renderer3D->render_wireframe) {
		glColor3f(0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::vector<Object*>::iterator temp = obj_vector.begin();
		while (temp != obj_vector.end()) {

			App->renderer3D->Render((*temp));
			
			temp++;
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	world_texture->Unbind();

	ImGui::Render();
	

	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
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
	return true;
}