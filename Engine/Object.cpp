#include "Object.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
void Object::AddComponentMesh(Mesh mesh)
{
	ComponentMesh* m = new ComponentMesh(mesh, this);
	obj_components.push_back(m);
	

}
void Object::AddComponentTransform()
{
	if (GetTransform() == nullptr) {
		aiVector3D pos = aiVector3D(0, 0, 0);
		aiQuaternion rot = aiQuaternion(1, 0, 0, 0);
		aiVector3D scale = aiVector3D(1, 1, 1);
		AddComponentTransform(pos, rot, scale);

	}

}
void Object::AddComponentTransform(aiVector3D pos, aiQuaternion rot, aiVector3D scale)
{
	if (GetTransform() == nullptr) {
		ComponentTransform* trans = new ComponentTransform(pos, rot, scale, this);
		obj_components.push_back(trans);
	}

}
void Object::AddComponentMaterial(Texture * tex)
{
	ComponentMaterial* mat = new ComponentMaterial(tex, this);
	obj_components.push_back(mat);


}
void Object::AddComponentCamera()
{
	ComponentCamera* mat = new ComponentCamera(this);
	obj_components.push_back(mat);

}
void Object::Update() {

	std::vector<Component*>::iterator item = obj_components.begin();
	if (obj_components.size() > 0) {
		while (item != obj_components.end()) {
			(*item)->UpdateComponent();
			item++;
		}
	}
	std::vector<Object*>::iterator obj_item = obj_childs.begin(); 
	if (obj_childs.size() > 0) {
		while (obj_item != obj_childs.end()) {
			(*obj_item)->Update();
			obj_item++;
		}
	}

	

}

ComponentMesh * Object::GetMesh()
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

ComponentMaterial * Object::GetMaterial()
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

ComponentTransform * Object::GetTransform()
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

ComponentCamera * Object::GetCamera()
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
