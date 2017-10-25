#include "ComponentCamera.h"
#include "Application.h"


ComponentCamera::ComponentCamera(GameObject* obj_parent)
{
	SetParent(obj_parent);
	comp_type = ComponentType::camera;
	cam_frustum.SetViewPlaneDistances(1, 15);

	//Set frustum looking to GameObject Z axis
	ComponentTransform* parent_transform = GetParent()->GetTransform();

	cam_frustum.SetPos(parent_transform->translation);
	cam_frustum.SetFront(parent_transform->GetMatrix().Col3(2));
	cam_frustum.SetUp(parent_transform->GetMatrix().Col3(1));


	cam_frustum.SetVerticalFovAndAspectRatio(90 * DEGTORAD, App->window->GetAspectRatio());
	cam_frustum.SetHorizontalFovAndAspectRatio(2 * math::Atan(math::Tan(cam_frustum.VerticalFov()/ 2) * App->window->GetAspectRatio()),App->window->GetAspectRatio());

	
}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::UpdateComponent()
{
	ComponentTransform* parent_transform = GetParent()->GetTransform();
	cam_frustum.SetPos(parent_transform->translation);

	cam_frustum.SetFront(parent_transform->GetMatrix().Col3(2));
	cam_frustum.SetUp(parent_transform->GetMatrix().Col3(1));

	float color[4] = { 0.7f, 0.7f, 0.7f, 1 };
	cam_frustum.Draw(3.0f,color );

}


