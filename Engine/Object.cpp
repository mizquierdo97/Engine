#include "Object.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
GameObject::~GameObject()
{

	auto it_non_static = std::find(App->world->non_static_list.begin(), App->world->non_static_list.end(), this);
	if (it_non_static != App->world->non_static_list.end())
		App->world->non_static_list.erase(it_non_static);
	auto it_static = std::find(App->world->static_list.begin(), App->world->static_list.end(), this);
	if (it_static != App->world->static_list.end())
		App->world->static_list.erase(it_static);


	if (obj_childs.size()) {
		std::vector<GameObject*>::iterator item = obj_childs.begin();
		while (item != obj_childs.end()) {
			RELEASE((*item));
			item++;
		}
		obj_childs.clear();
	}


	if (obj_components.size()) {
		std::vector<Component*>::iterator item_comp = obj_components.begin();
		while (item_comp != obj_components.end()) {
			RELEASE((*item_comp));
			item_comp++;
		}
		obj_components.clear();
	}	

}
void GameObject::AddComponentMesh(UUID uuid)
{
	ComponentMesh* m = new ComponentMesh(uuid, this);
	//m->path_name = mesh.mesh_path;
	obj_components.push_back(m);
	

}
void GameObject::AddComponentTransform()
{
	if (GetTransform() == nullptr) {
		aiVector3D pos = aiVector3D(0, 0, 0);
		aiQuaternion rot = aiQuaternion(1, 0, 0, 0);
		aiVector3D scale = aiVector3D(1, 1, 1);
		AddComponentTransform(pos, rot, scale);
		
	}

}
void GameObject::AddComponentTransform(aiVector3D pos, aiQuaternion rot, aiVector3D scale)
{
	if (GetTransform() == nullptr) {
		ComponentTransform* trans = new ComponentTransform(pos, rot, scale, this);
		obj_components.push_back(trans);
	}

}
void GameObject::AddComponentTransform(float3 pos, float4 rot, float3 scale)
{
	if (GetTransform() == nullptr) {
		ComponentTransform* trans = new ComponentTransform(pos, rot, scale, this);
		obj_components.push_back(trans);
	}

}
void GameObject::AddComponentMaterial(UUID  uuid)
{
	ComponentMaterial* mat = new ComponentMaterial(uuid, this);
	obj_components.push_back(mat);


}
void GameObject::AddComponentCamera()
{
	ComponentCamera* mat = new ComponentCamera(this);
	obj_components.push_back(mat);

}
void GameObject::AddComponentCamera(Frustum frus)
{
	ComponentCamera* mat = new ComponentCamera(this,frus);
	obj_components.push_back(mat);
}
void GameObject::Update() {

	std::vector<Component*>::iterator item = obj_components.begin();
	if (obj_components.size() > 0) {
		while (item != obj_components.end()) {
			(*item)->UpdateComponent();
			item++;
		}
	}
	std::vector<GameObject*>::iterator obj_item = obj_childs.begin(); 
	if (obj_childs.size() > 0) {
		while (obj_item != obj_childs.end()) {
			(*obj_item)->Update();
			obj_item++;
		}
	}
}


void GameObject::Draw()
{
	if (GetMesh() != nullptr) {
		ComponentMesh* m = GetMesh();
		if (App->renderer3D->render_fill) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			App->renderer3D->Render(m);
		}
		if (App->renderer3D->render_wireframe) {
			glColor3f(0.0f, 0.0f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			App->renderer3D->Render(m);
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		Mesh mesh = ((ResourceMesh*)GetMesh()->GetResource())->obj_mesh;
		if (mesh.norms != nullptr && App->gui->show_normals)
			App->physics->DrawNormals(m);
		
	}

}



Component * GameObject::FindComponentbytype(ComponentType type) const
{
	for(int i = 0; i < obj_components.size();i++)
	{
		if (obj_components[i]->comp_type == type)
		{
			return obj_components[i];
	
		}
	}
	return nullptr;
}
void GameObject::ToDelete()
{
	to_delete = true;

	std::vector<GameObject*>::iterator item = obj_childs.begin();
	/*while (item != obj_childs.end()) {
		(*item)->ToDelete();
		item++;
	}*/
}
void GameObject::SetStatic(bool val)
{

	is_static = val;

	if (is_static) {
		App->world->quadtree.Insert(this);
		App->world->static_list.push_back(this);
		App->world->non_static_list.remove(this);
	}
	else {
		App->world->quadtree.Erase(this);
		App->world->static_list.remove(this);
		App->world->non_static_list.push_back(this);
	}

	for (int i = 0; i < obj_childs.size(); ++i) {
		obj_childs[i]->SetStatic(is_static);
	}
	
}
ComponentMesh * GameObject::GetMesh()
{
	std::vector<Component*>::iterator item = obj_components.begin();
	while (item != obj_components.end()) {
		if ((*item)->comp_type == ComponentType::mesh) {
			ComponentMesh* ret = (ComponentMesh*)(*item);
			return ret;
		}

		item++;
	}

	return nullptr;
}

ComponentMaterial * GameObject::GetMaterial()
{
	std::vector<Component*>::iterator item = obj_components.begin();
	while (item != obj_components.end()) {
		if ((*item)->comp_type == ComponentType::material) {
			ComponentMaterial* ret = (ComponentMaterial*)(*item);
			return ret;
		}

		item++;
	}
	return nullptr;
}

ComponentTransform * GameObject::GetTransform()
{
	std::vector<Component*>::iterator item = obj_components.begin();
	while (item != obj_components.end()) {
		if ((*item)->comp_type == ComponentType::transform) {
			ComponentTransform* ret = (ComponentTransform*)(*item);
			return ret;
		}

		item++;
	}
	return nullptr;
}

ComponentCamera * GameObject::GetCamera()
{
	std::vector<Component*>::iterator item = obj_components.begin();
	while (item != obj_components.end()) {
		if ((*item)->comp_type == ComponentType::camera) {
			ComponentCamera* ret = (ComponentCamera*)(*item);
			return ret;
		}

		item++;
	}
	return nullptr;
}


