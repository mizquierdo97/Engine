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
	ComponentTransform(float3, float4, float3, GameObject*);
	~ComponentTransform() {};

	void UpdateComponent();
	void ShowInspectorComponents();
	void LockProprtionScale(float3 before_scale);

	const float4x4 GetMatrix();
	const float4x4 GetMyMatrix();
public:
	float3 translation;
	float3 scale;
	Quat rotation;
	float4x4 matrix = float4x4::identity;
	
	bool lock_scale_prop;
};