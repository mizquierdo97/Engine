#pragma once
#include "Component.h"
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "ModuleAssimp.h"

class ComponentTransform : public Component{
public:
	ComponentTransform() {};
	ComponentTransform(aiVector3D, aiQuaternion, aiVector3D, GameObject*);
	~ComponentTransform() {};

	void UpdateComponent();
	void ShowInspectorComponents();
	const float4x4 GetMatrix();
	const float4x4 GetMyMatrix();
public:
	float3 translation;
	float3 scale;
	Quat rotation;
	float4x4 matrix = float4x4::identity;
	bool header_open;
};