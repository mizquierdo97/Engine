#pragma once
#include "Component.h"
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "ModuleAssimp.h"

class ComponentTransform : public Component{
public:
	ComponentTransform() {};
	ComponentTransform(aiVector3D, aiQuaternion, aiVector3D);
	~ComponentTransform() {};

	void UpdateComponent();
	void ShowInspectorComponents();
public:
	float3 translation;
	float3 scale;
	Quat rotation;

	bool header_open;
};