#include "Object.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
void GameObject::AddComponentMesh(Mesh mesh)
{
	ComponentMesh* m = new ComponentMesh(mesh, this);
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
void GameObject::AddComponentMaterial(Texture * tex)
{
	ComponentMaterial* mat = new ComponentMaterial(tex, this);
	obj_components.push_back(mat);


}
void GameObject::AddComponentCamera()
{
	ComponentCamera* mat = new ComponentCamera(this);
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
