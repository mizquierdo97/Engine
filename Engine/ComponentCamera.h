#pragma once
#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "ComponentTransform.h"
#include "Object.h"
#include "ModuleWindow.h"


class Data;
class ComponentCamera : public Component {
public:
	ComponentCamera();
	ComponentCamera(GameObject*);
	~ComponentCamera();

	void UpdateComponent();
	void SaveComponentScene(Data* data) {};
	void ShowInspectorComponents();

public: 
	Frustum cam_frustum;
	bool frustum_culling = true;

};