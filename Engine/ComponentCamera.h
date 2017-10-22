#pragma once

#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"
class ComponentCamera : public Component {

	ComponentCamera(){

	}
	~ComponentCamera() {

	}

	void UpdateComponent();


public: 
	Frustum cam_frustum;
	bool frustum_culling = true;

};