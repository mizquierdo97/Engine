#include "Object.h"
#include "Component.h"
void Object::Update() {

	std::vector<Component*>::iterator item = obj_components.begin();
	while (item != obj_components.end()) {
		(*item)->UpdateComponent();
		item++;
	}

}