#pragma once
#include "Globals.h"
#include "Module.h"
#include "Data.h"
enum ComponentType {
	
	transform,
	mesh,
	material,
	camera,

};

class GameObject;
class Data;
class Component {

public:
	Component() {

	}
	~Component() {

	}

	void DeleteComponent();

	virtual void UpdateComponent();
	virtual void ShowInspectorComponents();
	virtual void SaveComponentScene(Data*) {};

	bool IsEnabled() {
		return enabled;
	}
	void SetEnable(bool val) {
		enabled = val;
	}

	GameObject* GetParent() {
		return parent;
	}
	void SetParent(GameObject* val) {
		parent = val;
	}


public:

	ComponentType comp_type;

private:
	bool enabled;
	GameObject* parent;

	
};
