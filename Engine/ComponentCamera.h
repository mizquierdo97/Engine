#pragma once
#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "ComponentTransform.h"
#include "Object.h"
#include "ModuleWindow.h"

class ComponentCamera : public Component {
public:
	ComponentCamera() {};
	ComponentCamera(GameObject*);
	~ComponentCamera();

	void UpdateComponent();


public: 
	Frustum cam_frustum;
	bool frustum_culling = true;

};