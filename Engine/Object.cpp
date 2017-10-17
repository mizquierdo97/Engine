#include "Object.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
void Object::AddComponentMesh(Mesh mesh)
{
	ComponentMesh* m = new ComponentMesh(mesh, this);
	obj_components.push_back(m);
	

}
void Object::AddComponentMaterial(Texture * tex)
{
	ComponentMaterial* mat = new ComponentMaterial(tex, this);
	obj_components.push_back(mat);


}
void Object::Update() {

	std::vector<Component*>::iterator item = obj_components.begin();
	while (item != obj_components.end()) {
		(*item)->UpdateComponent();
		item++;
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
