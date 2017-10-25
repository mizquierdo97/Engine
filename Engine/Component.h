#pragma once
#include "Globals.h"
#include "Module.h"

enum ComponentType {
	
	transform,
	mesh,
	material,
	camera,

};

class GameObject;
class Component {

public:
	Component() {

	}
	~Component() {

	}

	void DeleteComponent();

	virtual void UpdateComponent();
	virtual void ShowInspectorComponents();
public:

	ComponentType comp_type;
	bool enabled;
	GameObject* parent;

	
};
