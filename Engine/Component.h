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
class Resource;
class Data;
class Component {

public:
	Component() {

	}
	virtual ~Component() {

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
	Resource* GetResource()const;

public:

	ComponentType comp_type;
	UUID res_uuid;
private:
	bool enabled;
	GameObject* parent;

protected:
	bool header_open;
	
};
