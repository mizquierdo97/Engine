#include "Application.h"
#include "ModuleWorld.h"
#include "ImGui\ImGuizmo.h"
#include "imgui_dock.h"
#include "Object.h"
#include "ComponentMesh.h"
#include <string>
#include "Data.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include <map>
#include "ImGui\imgui_internal.h"

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

ModuleWorld::ModuleWorld(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleWorld::~ModuleWorld()
{
}

bool ModuleWorld::Init()
{
	quadtree.SetBoundaries(AABB(float3(-50, -50, -50), float3(50, 40, 50)));
	return true;
}


bool ModuleWorld::Start() {

	//Load GUI Textures
	App->filesystem->image_importer->loadTextureFromFile("play.png", &play_tex, false);
	App->filesystem->image_importer->loadTextureFromFile("stop.png", &stop_tex, false);
	App->filesystem->image_importer->loadTextureFromFile("pause.png", &pause_tex, false);
	App->filesystem->image_importer->loadTextureFromFile("frame.png", &nframe_tex, false);

	//Create world Texture
	world_texture = new Texture();
	world_texture->Create(nullptr, App->window->width, App->window->height);
		
	//Create Camera
	GameObject* obj = CreateObject();
	
	obj->AddComponentCamera();
	obj->SetName("Camera");
	
	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{
	static float time = 0;
	time += dt;
	if (time >= 5.0f) {
		App->filesystem->CheckFilesUpdates();
		time = 0;
	}
	//IF a files is dropped on the screen
	if (App->input->file_dropped) {
		FileDropped();
	}
		
	
	world_texture->Bind();
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN){
		for (int i = 0; i < obj_vector.size(); i++) {
			obj_vector[i]->SetStatic(!obj_vector[i]->IsStatic());
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		Operation = ImGuizmo::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		Operation = ImGuizmo::ROTATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		Operation = ImGuizmo::SCALE;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float dt)
{
	
	//Iterate Object List and Render
	It_Update();
	It_Render();

	if(App->renderer3D->debug_draw)
	DebugDraw();

	world_texture->Unbind();
	//Render Imgui
	ImGui::Render();	

	DeleteObject(obj_vector);

	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{

	std::vector<GameObject*>::iterator item = obj_vector.begin();	

	while (item != obj_vector.end()) {
		RELEASE((*item));
		item++;
	}
	obj_vector.clear();	
	
	uuid_vect.clear();
	static_list.clear();
	non_static_list.clear();

	RELEASE(world_texture);
	RELEASE(play_tex);
	RELEASE(pause_tex);
	RELEASE(nframe_tex);
	RELEASE(stop_tex);

	return true;
}



void ModuleWorld::FileDropped()
{
	std::string path = App->input->dropped_filedir;	

	std::transform(path.begin(), path.end(), path.begin(), ::tolower);	
	std::string file_name = ASSETS_PATH + GetFileNameExtension(path);
	bool file_exists = ExistsFile(file_name);
	CopyFile(path.c_str(), file_name.c_str(),false);	
	if (!file_exists) {
		App->filesystem->ImportFile(App->filesystem->CreateMeta(file_name.c_str()));
		}


	App->input->file_dropped = false;
}

void ModuleWorld::It_Render()
{
	//Render polygons
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		using_octree = !using_octree;
	}
	
	if (using_octree) {
		std::vector<GameObject*> objects;

		//GetThe camera Camera
		ComponentCamera* cam = App->renderer3D->GetActiveCamera();
		
		//COLLECT OBJECTS
		if (cam != nullptr && cam->frustum_culling == true)
			quadtree.root->CollectIntersectionsFrus(objects, cam->cam_frustum);
		else
			quadtree.CollectObjects(objects);

		//RENDER STATIC OBJECTS
		for (std::vector<GameObject*>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it) {
			if ((*it)->IsEnabled())
				(*it)->Draw();
		}

		//RENDER NON-STATIC OBJECTS
		ComponentCamera* active_camera = App->renderer3D->GetActiveCamera();

		for (std::list<GameObject*>::reverse_iterator it = non_static_list.rbegin(); it != non_static_list.rend(); ++it) {
			AABB bbox = (*it)->GetGlobalBBox();
			if (active_camera != nullptr &&(active_camera->cam_frustum.ContainsAaBox(bbox) != -1 || !active_camera->frustum_culling)) {
				if ((*it)->IsEnabled())
					(*it)->Draw();
			}
		}
		
	}
	
	glColor3f(1.0f, 1.0f, 1.0f);

}

void ModuleWorld::It_Update()
{
	std::vector<GameObject*>::iterator temp = obj_vector.begin();

	while (temp != obj_vector.end()) {
		(*temp)->Update();
		temp++;
	}

}

void ModuleWorld::DebugDraw(){

	std::vector<const QuadtreeNode*> boxes;
	quadtree.CollectBoxes(boxes);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		see_octree = !see_octree;
	}

	if (see_octree) {
		for (std::vector<const QuadtreeNode*>::const_iterator it = boxes.begin(); it != boxes.end(); ++it)
			App->renderer3D->DebugDraw((*it)->bounds, Yellow);
	}
}


void ModuleWorld::ClearScene()
{
	std::vector<GameObject*>::iterator item = obj_vector.begin();
	while (item != obj_vector.end()) {
		RELEASE(*item);
		item++;
	}
	obj_vector.clear();
	uuid_vect.clear();
	non_static_list.clear();
	static_list.clear();

}

//LOAD SCENE
void ModuleWorld::LoadScene(const char* name) {

	//FIRST CLEAR THE LAST SCENE and QUADTREE
	ClearScene();
	App->world->SetSelectedObject(nullptr);
	quadtree.Clear();
	quadtree.SetBoundaries(AABB(float3(-50, -50, -50), float3(50, 50, 50)));


	std::string scene_name = name;
	Data scene_data;
	int i = 0;
	//LOAD JSON FILE
	if (scene_data.LoadJSON(scene_name + ".json")) {
		scene_data.EnterSection("GameObjects");

		//FOR EVERY GAME_OBJECT...
		while (scene_data.EnterSection("Object_"+ std::to_string(i++))) {

			UUID test_id; //UUID of the object
			UuidFromStringA((RPC_CSTR)scene_data.GetString("UUID").c_str(), &test_id);
			
			bool exists = false;
			std::vector<std::pair<GameObject*, UUID>>::iterator item = uuid_vect.begin();

			while (item != uuid_vect.end()) {
				if ((*item).second == test_id) //Check if there's an object with the same UUID
					exists = true;
				item++;
			}
			
			//IF DONT EXISTS... CREATE IT!!
			if (!exists) {
				GameObject* go = new GameObject();

				//Load Object Data
				LoadSceneGoData(scene_data, go);	//Name, Transform, UUID, parent UUID...			

				if (scene_data.EnterSection("Components")) {

					//LOAD CAMERA OPTIONS
					LoadSceneCamera(scene_data, go);
					//LOAD SCENE MESH
					LoadSceneMesh(scene_data, go);					
					//LOAD SCENE MATERIAL
					LoadSceneMaterial(scene_data, go);
					
					scene_data.LeaveSection();
				}	
				
				std::pair<GameObject*, UUID> temp_pair;
				temp_pair.first = go;
				temp_pair.second = go->obj_uuid;
				App->world->uuid_vect.push_back(temp_pair); //PUSH OBJ IN THE UUID VECT

				if(go->IsStatic())
					App->world->static_list.push_back(go); //PUSH IT IN THE STATIC LIST TOO
				else
				App->world->non_static_list.push_back(go); //OR IN THE NON STATIC
				
			}
			scene_data.LeaveSection();
		}
		scene_data.LeaveSection();		
	}
	//Set parents and childs correctly
	RedistributeGameObjects();

	//CREATE AABB
	std::vector<GameObject*>::iterator item = obj_vector.begin();

	while (item != obj_vector.end()) {
		if(*item != nullptr)
		RecursiveCreateAABB(*item);
		item++;
	}
}


//SAVE SCENE
void ModuleWorld::SaveScene(const char* name) const
{
	std::string scene_name = name;
	Data scene_data;
	int i = 0;
	scene_data.CreateSection("GameObjects");
	App->world->RecursiveSaveScene(obj_vector, &scene_data, &i);
	scene_data.CloseSection();
	scene_data.SaveAsJSON(scene_name + ".json");
}


void ModuleWorld::RecursiveSaveScene(std::vector<GameObject*> vect, Data* data, int* i) {

	std::vector<GameObject*>::iterator item = vect.begin();

	while (item != vect.end()) {

		char *str;
		data->CreateSection("Object_" + std::to_string(*i));
		(*i) += 1;

		UuidToStringA(&(*item)->obj_uuid, (RPC_CSTR*)&str);
		data->AddString("UUID", str);
		data->AddBool("Static", (*item)->IsStatic());

		if ((*item)->obj_parent != nullptr) {
			UuidToStringA(&(*item)->obj_parent->obj_uuid, (RPC_CSTR*)&str);
		}
		else {
			//PROBABLY WOULD CHANGE THIS
			UuidCreateNil(&(*item)->parent_uuid);////
			UuidToStringA(&(*item)->parent_uuid, (RPC_CSTR*)&str);
		}
		data->AddString("Parent UUID", str);
		ComponentTransform* temp_trans = (*item)->GetTransform();

		data->AddString("Name", (*item)->GetName());
		data->AddVector3("Translation", temp_trans->translation);

		float4 rotation = float4(temp_trans->rotation.x, temp_trans->rotation.y, temp_trans->rotation.z, temp_trans->rotation.w);
		data->AddVector4("Rotation", rotation);
		data->AddVector3("Scale", temp_trans->scale);

		data->CreateSection("Components");


		for (int n = 0; n < (*item)->obj_components.size(); n++) {

			(*item)->obj_components[n]->SaveComponentScene(data);

		}
		data->CloseSection();
		data->CloseSection();
		RecursiveSaveScene((*item)->obj_childs, data, i);

		item++;
	}
}

//DELETE OBJECT
void ModuleWorld::DeleteObject(std::vector<GameObject*> vect)
{
	std::vector<GameObject*>::iterator item = vect.begin();
	GameObject* obj;
	while (item != vect.end()) {
		DeleteObject((*item)->obj_childs);
		if ((*item)->to_delete) {
			if ((*item)->obj_parent == nullptr) {
				auto it = std::find(App->world->obj_vector.begin(), App->world->obj_vector.end(), (*item));
				if (it != App->world->obj_vector.end())
					App->world->obj_vector.erase(it);
			}

			else {

				auto it = std::find((*item)->obj_parent->obj_childs.begin(), (*item)->obj_parent->obj_childs.end(), (*item));
				if (it != (*item)->obj_parent->obj_childs.end())
					(*item)->obj_parent->obj_childs.erase(it);
			}
			RELEASE(*item);
			break;
		}
		item++;
	}
}


void ModuleWorld::RecursiveCreateAABB(GameObject* obj) {
	AABB* temp = new AABB();

	if (obj->GetMesh() != nullptr) {
		ResourceMesh* res = (ResourceMesh*)obj->GetMesh()->GetResource();
		temp->SetFrom((float3*)res->obj_mesh.vertexs, res->obj_mesh.num_vertexs);
		obj->SetGlobalBox(*temp);
		UpdateAABB(obj);		
	}
	RELEASE(temp);

	std::vector<GameObject*>::iterator item = obj->obj_childs.begin();

	while (item != obj->obj_childs.end()) {
		RecursiveCreateAABB(*item);
		item++;
	}
}


void ModuleWorld::RedistributeGameObjects()
{
	RPC_STATUS stat;
	UUID nil;
	UuidCreateNil(&nil);
	std::vector<std::pair<GameObject*, UUID>>::iterator item = uuid_vect.begin();
	while (item != uuid_vect.end()) {
		UUID cmp = (*item).second;

		if ((*item).first->parent_uuid == nil) {
			App->world->obj_vector.push_back((*item).first);
		}
		else {
			std::vector<std::pair<GameObject*, UUID>>::iterator item2 = uuid_vect.begin();
			while (item2 != uuid_vect.end()) {
				if ((*item).first->parent_uuid == (*item2).second) {
					(*item2).first->obj_childs.push_back((*item).first);
					(*item).first->obj_parent = (*item2).first;
				}
				item2++;
			}
		}
		item++;
	}
}

void ModuleWorld::LoadSceneGoData(Data scene_data, GameObject * go)
{
	UuidFromStringA((RPC_CSTR)scene_data.GetString("UUID").c_str(), &go->obj_uuid);
	UuidFromStringA((RPC_CSTR)scene_data.GetString("Parent UUID").c_str(), &go->parent_uuid);

	go->SetName(scene_data.GetString("Name"));
	go->SetStatic(scene_data.GetBool("Static"));
	float3 trans = scene_data.GetVector3f("Translation");
	float4 rot = scene_data.GetVector4f("Rotation");
	float3 scale = scene_data.GetVector3f("Scale");

	go->AddComponentTransform(trans, rot, scale);

}

void ModuleWorld::LoadSceneMesh(Data scene_data, GameObject* go)
{
	if (scene_data.EnterSection("Mesh")) {
		scene_data.GetUInt("Num Vertexs");
		uint ranges[4] = { scene_data.GetUInt("Num Vertexs"),scene_data.GetUInt("Num Indices"),  scene_data.GetBool("Texture"), scene_data.GetBool("Norms") };
		std::string mesh_path = scene_data.GetString("Mesh_Path").c_str();
		std::string library_path = MESHES_PATH + GetFileName(mesh_path) + ".mesh";
		UUID obj_uuid = App->resources->FindImported(library_path.c_str());
		ResourceMesh* res = (ResourceMesh*)App->resources->Get(obj_uuid);
		if (res != nullptr) {
			res->LoadToMemory();
			go->AddComponentMesh(obj_uuid);
		}
	
		scene_data.LeaveSection();
	}

}

void ModuleWorld::LoadSceneMaterial(Data scene_data, GameObject* go)
{
	if (scene_data.EnterSection("Material")) {
		std::string texture_path = scene_data.GetString("Texture Path");
		std::string library_path = MATERIALS_PATH + GetFileName(texture_path) + ".dds";


		if (ExistsFile(library_path)) {
			UUID obj_uuid = App->resources->FindImported(library_path.c_str());
			ResourceTexture* res = (ResourceTexture*)App->resources->Get(obj_uuid);
			if (res != nullptr) {
				res->LoadToMemory();
				go->AddComponentMaterial(obj_uuid);
			}		
		}
		else {
			LOG("CANT FIND %s TEXTURE", texture_path.c_str());
		}
		scene_data.LeaveSection();

	}

}

void ModuleWorld::LoadSceneCamera(Data scene_data, GameObject * go )
{
	if (scene_data.EnterSection("Camera")) {
		Frustum new_frust;
		new_frust.pos = scene_data.GetVector3f("Position");
		new_frust.front = scene_data.GetVector3f("Front");
		new_frust.up = scene_data.GetVector3f("Up");
		new_frust.nearPlaneDistance = scene_data.GetFloat("Near");
		new_frust.farPlaneDistance = scene_data.GetFloat("Far");
		new_frust.verticalFov = scene_data.GetFloat("Vertical FOV");
		new_frust.horizontalFov = scene_data.GetFloat("Horizontal FOV");

		go->AddComponentCamera(new_frust);
		scene_data.LeaveSection();
	}

}


bool ModuleWorld::Options()
{
	if (ImGui::BeginDock("Scene", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {


		ImGui::Checkbox("Use Octree", &using_octree); ImGui::SameLine();
		ImGui::Checkbox("See Octree", &see_octree); ImGui::SameLine();
		if (App->renderer3D->GetActiveCamera() != nullptr)
		ImGui::Checkbox("Frustum Culling", &App->renderer3D->GetActiveCamera()->frustum_culling);

		ImVec2 size2 = ImGui::GetContentRegionAvail();
		ImGui::SameLine(App->world->world_tex_vec.z / 2.5);
		if (ImGui::ImageButton((void*)play_tex->GetTexture(), ImVec2(35, 26)))
			App->SetGameMode(PLAY);

		ImGui::SameLine();
		if (ImGui::ImageButton((void*)pause_tex->GetTexture(), ImVec2(35, 26)))
			App->SetGameMode(PAUSE);

		ImGui::SameLine();
		if (ImGui::ImageButton((void*)stop_tex->GetTexture(), ImVec2(35, 26)))
			App->SetGameMode(STOP);

		ImGui::SameLine();

		if (ImGui::ImageButton((void*)nframe_tex->GetTexture(), ImVec2(35, 26)))
			App->SetGameMode(NEXT_FRAME);

		ImVec2 size = ImGui::GetContentRegionAvail();
		ImVec2 pos = ImGui::GetCurrentWindow()->Pos;
		auto im_context = ImGui::GetCurrentContext();
		auto focused_window = im_context->NavWindow;
		auto this_window = ImGui::GetCurrentWindowRead();
		
		world_tex_vec.x = pos.x;
		world_tex_vec.y = pos.y;
		world_tex_vec.z = size.x;
		world_tex_vec.w = size.y;

		Focus(false);
		if (focused_window == this_window)
			Focus(true);

		ImGui::Image((void*)world_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::EndDock();

	
	if (ImGui::BeginDock("Hierarchy", false, false, false)) {

		static int selection_mask = 0; // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.

		int i = -1;
		int node_selected = -1;
		bool open_window = false;
		
		HierarchyRecurs(obj_vector, &node_selected, i, selection_mask, &open_window);

		if (node_selected != -1)
		{
			// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
			if (ImGui::GetIO().KeyCtrl)
				selection_mask ^= (1 << node_selected);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
				selection_mask = (1 << node_selected);           // Click to single-select
		}


		ImGui::EndDock();
	}
	return true;
}

int ModuleWorld::HierarchyRecurs(std::vector<GameObject*> vector, int* node_selected, int i, int selection_mask, bool* open_window)
{
	int node_clicked = -1;

	int temp_i = 0;
	std::vector<GameObject*>::iterator item = vector.begin();
	while (item != vector.end())
	{
		i++;
		ImGui::PushID(i);
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);

		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, (*item)->GetName().c_str());

		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
			node_selected[0] = i;
			selected_object = (*item);

			if(ImGui::IsItemClicked(1)){
				open_window[0] = true;
			}

		}

		if (node_open)
		{
			i = HierarchyRecurs((*item)->obj_childs, node_selected, i, selection_mask, open_window);

			ImGui::TreePop();
		}
		ImGui::PopID();
		item++;
	}

	return i;
}
