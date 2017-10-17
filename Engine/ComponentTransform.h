#pragma once
#include "Component.h"
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"


class ComponentTransform : public Component{
public:


public:
	float3 translation;
	float3 scale;
	Quat rotation;
};