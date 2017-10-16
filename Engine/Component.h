#pragma once
#include "Globals.h"
#include "Module.h"

enum ComponentType {
	
	transform,
	mesh,
	material,

};

class Component {

public:
	Component() {

	}
	~Component() {

	}

	void DeleteComponent();

public:

	ComponentType comp_type;
	bool enabled;
	
	
};
