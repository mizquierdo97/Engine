#include "Application.h"
#include "ModuleWorld.h"

#include "imgui_dock.h"
#include "Object.h"
#include "ComponentMesh.h"
#include <string>
#include "Data.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include <map>

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
	quadtree.SetBoundaries(AABB(float3(-50, -50, -50), float3(50, 50, 50)));
	return true;
}


bool ModuleWorld::Start() {
	world_texture = new Texture();
	world_texture->Create(nullptr, App->window->width, App->window->height);
		
	CreateObject();
	obj_vector[0]->AddComponentTransform();
	obj_vector[0]->AddComponentCamera();
	
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
	It_Update();
	It_Render();

	DebugDraw();

	world_texture->Unbind();
	//Render Imgui
	ImGui::Render();	

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
	
	RELEASE(world_texture);
	return true;
}



void ModuleWorld::FileDropped()
{
	std::string path = App->input->dropped_filedir;
	std::string temp;

	std::transform(path.begin(), path.end(), path.begin(), ::tolower);
	temp = path.substr(path.length() - 3, 3);


	if (!strcmp((char*)temp.c_str(), "fbx"))
		App->assimp->ImportMesh(path.c_str());

	else if (!strcmp((char*)temp.c_str(), "png") || !strcmp((char*)temp.c_str(), "jpg"))
		App->gui->path_list.push_back(path);

	App->input->file_dropped = false;
}

void ModuleWorld::It_Render()
{
	//Render polygons


	//RENDER MESHES
	std::vector< GameObject*> objects;

	// we do frustum culling or not ?
	ComponentCamera* cam = App->renderer3D->GetActiveCamera();

	// TODO first draw all opaque geom then translucent from far to near
	if (cam->frustum_culling == true)
		quadtree.root->CollectIntersectionsFrus(objects, cam->cam_frustum);
	else
		quadtree.CollectObjects(objects);

	for (std::vector< GameObject*>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it)
		if ((*it)->IsEnabled())
			(*it)->Draw();
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

void ModuleWorld::DebugDraw()
{

	std::vector<const QuadtreeNode*> boxes;
	quadtree.CollectBoxes(boxes);

	for (std::vector<const QuadtreeNode*>::const_iterator it = boxes.begin(); it != boxes.end(); ++it)
		App->renderer3D->DebugDraw((*it)->bounds, Yellow);

}
void ModuleWorld::LoadScene() {
	Data scene_data;
	int i = 0;
	if (scene_data.LoadJSON("Scene.json")) {
		scene_data.EnterSection("GameObjects");
		while (scene_data.EnterSection("Object_"+ std::to_string(i++))) {

			UUID test_id;
			UuidFromStringA((RPC_CSTR)scene_data.GetString("UUID").c_str(), &test_id);

			//Look if exists
			bool exists = false;
			std::vector<std::pair<GameObject*, UUID>>::iterator item = uuid_vect.begin();
			while (item != uuid_vect.end()) {
				if ((*item).second == test_id)
					exists = true;
				item++;
			}

			if (!exists) {
				GameObject* go = new GameObject();

				//Load Object Data
				LoadSceneGoData(scene_data, go);				

				if (scene_data.EnterSection("Components")) {

					//LOAD SCENE MESH
					LoadSceneMesh(scene_data, go);
					
					//LOAD SCENE MATERIAL
					LoadSceneMaterial(scene_data, go);
					
					scene_data.LeaveSection();
				}				
			}
			scene_data.LeaveSection();
		}
		scene_data.LeaveSection();		
	}
	//Set parents and childs correctly
	RedistributeGameObjects();
}

void ModuleWorld::SaveScene() const
{
	Data scene_data;
	int i = 0;
	scene_data.CreateSection("GameObjects");
	App->world->RecursiveSaveScene(obj_vector, &scene_data,&i);
	scene_data.CloseSection();
	scene_data.SaveAsJSON("Scene.json");
}


void ModuleWorld::RecursiveSaveScene(std::vector<GameObject*> vect,Data* data, int* i) {
	std::vector<GameObject*>::iterator item = vect.begin();
	while (item != vect.end()) {

		char *str;
		data->CreateSection("Object_"+ std::to_string(*i));
		(*i) += 1;

		UuidToStringA(&(*item)->obj_uuid, (RPC_CSTR*)&str);		
		data->AddString("UUID", str);	

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

		data->AddString("Name",(*item)->GetName());		
		data->AddVector3("Translation", temp_trans->translation);
		data->AddVector4("Rotation", temp_trans->rotation.CastToFloat4());
		data->AddVector3("Scale", temp_trans->scale);
		
		data->CreateSection("Components");
		for (int n = 0; n < (*item)->obj_components.size(); n++) {
			
			(*item)->obj_components[n]->SaveComponentScene(data);

		}
		data->CloseSection();
		data->CloseSection();
		RecursiveSaveScene((*item)->obj_childs, data,i);

		item++;
	}
}


bool ModuleWorld::Options()
{
	if (ImGui::BeginDock("Scene", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImVec4 temp = ImGui::GetSizeDock(2);
		world_tex_vec.x = temp.x;
		world_tex_vec.y = temp.y;
		world_tex_vec.z = temp.z;
		world_tex_vec.w = temp.w;


			ImGui::Image((void*)world_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
	}
	
	ImGui::EndDock();

	if (ImGui::BeginDock("Configuration", false, false, false)) {

		std::vector<GameObject*>::iterator item = obj_vector.begin();
		int num = 0;
		while(item != obj_vector.end())		
		{
			
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
			
			item++;
		}
	}
	
	ImGui::EndDock();

	if (ImGui::BeginDock("Hierarchy", false, false, false)) {

		static int selection_mask = 0; // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		
		int i = -1;
		int node_selected = -1;
		//std::vector<Object*> item = obj_vector.begin();
		HierarchyRecurs(obj_vector,&node_selected,i,selection_mask);

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

int ModuleWorld::HierarchyRecurs(std::vector<GameObject*> vector,int* node_selected, int i,int selection_mask)
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

		if (ImGui::IsItemClicked()) {
			//node_clicked = *i;
			node_selected[0] = i;
			selected_object = (*item);
			//return i;
		}

		if (node_open)
		{
			i = HierarchyRecurs((*item)->obj_childs,node_selected, i,selection_mask);

			ImGui::TreePop();
		}	
		ImGui::PopID();
		item++;
	}
	
	return i;
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

	float3 trans = scene_data.GetVector3f("Translation");
	float4 rot = scene_data.GetVector4f("Rotation");
	float3 scale = scene_data.GetVector3f("Scale");

	go->AddComponentTransform(trans, rot, scale);

	std::pair<GameObject*, UUID> temp_pair;
	temp_pair.first = go;
	temp_pair.second = go->obj_uuid;
	App->world->uuid_vect.push_back(temp_pair);

}

void ModuleWorld::LoadSceneMesh(Data scene_data, GameObject* go)
{
	if (scene_data.EnterSection("Mesh")) {
		scene_data.GetUInt("Num Vertexs");
		uint ranges[4] = { scene_data.GetUInt("Num Vertexs"),scene_data.GetUInt("Num Indices"),  scene_data.GetBool("Texture"), scene_data.GetBool("Norms") };
		Mesh m = App->filesystem->mesh_importer->LoadComponentMesh((char*)scene_data.GetString("Mesh_Path").c_str(), &ranges[0]);
		go->AddComponentMesh(m);
		RELEASE_ARRAY(m.indices);
		RELEASE_ARRAY(m.vertexs);
		RELEASE_ARRAY(m.norms);
		RELEASE_ARRAY(m.texture_coords);

		scene_data.LeaveSection();
	}

}

void ModuleWorld::LoadSceneMaterial(Data scene_data, GameObject* go)
{
	if (scene_data.EnterSection("Material")) {
		std::string mesh_path = scene_data.GetString("Mesh_Path");
		std::string library_path = MESHES_PATH + GetFileName(mesh_path) + ".dds";

		Texture* temp_tex = new Texture();
		if (ExistsFile(library_path)) {
			App->renderer3D->loadTextureFromFile((char*)library_path.c_str(), &temp_tex);
			go->AddComponentMaterial(temp_tex);
		}/*
		 else if (ExistsFile(mesh_path)) {
		 App->filesystem->ImportImage(mesh_path.c_str());
		 App->renderer3D->loadTextureFromFile(library_path.c_str(), &temp_tex);
		 go->AddComponentMaterial(temp_tex);
		 }*/
		else {
			LOG("CANT FIND %s TEXTURE", mesh_path.c_str());
		}
		scene_data.LeaveSection();

	}

}

GameObject * ModuleWorld::Raycast(const LineSegment & segment, float  &dist)const  
{
	dist = inf; 
	GameObject* Closest_object = nullptr;
	Recursivetest(segment, dist, &Closest_object);
	return Closest_object;
}



void ModuleWorld::Recursivetest(const LineSegment& segment, float& dist, GameObject** closest_object)const 
{
	std::map<float,GameObject*> obj;
	quadtree.root->CollectIntersections(obj, segment);
	std::vector<ComponentMesh*> meshes;
	GameObject* goTemp;
	for (std::map<float,GameObject*>::const_iterator iterator = obj.begin(); iterator != obj.end(); iterator++)
	{
		goTemp = iterator->second;
		
		meshes.push_back((goTemp)->GetMesh());		
		}


	for(int n =0;n<meshes.size();n++){
		ComponentMesh* oMesh = meshes[n];
		GameObject* go = oMesh->GetParent();
		ComponentTransform* transform = (ComponentTransform*)(goTemp)->GetTransform();
		Mesh objmesh = oMesh->obj_mesh;

		LineSegment local(segment);
		local.Transform(transform->GetMatrix().Inverted());

		Triangle triangle;
		for (int i = 0; i < objmesh.num_indices - 9;)
		{

			float point1[] = {
				objmesh.vertexs[objmesh.indices[i++]],
				objmesh.vertexs[objmesh.indices[i++]],
				objmesh.vertexs[objmesh.indices[i++]]
			};

			float point2[] = {
				objmesh.vertexs[objmesh.indices[i++]],
				objmesh.vertexs[objmesh.indices[i++]],
				objmesh.vertexs[objmesh.indices[i++]]
			};

			float point3[] = {
				objmesh.vertexs[objmesh.indices[i++]],
				objmesh.vertexs[objmesh.indices[i++]],
				objmesh.vertexs[objmesh.indices[i++]]
			};


			triangle.a.Set(point1);
			triangle.b.Set(point2);
			triangle.c.Set(point3);
			
			float localdistance = 0;
			float3 localhitpoint;

			if (local.Intersects(triangle, &localdistance, &localhitpoint))
			{
				if (localdistance < dist) {

					dist = localdistance;
					closest_object[0] = go;
					
				}
			}
			}
		}
	}

