#include "ComponentCamera.h"
#include "Application.h"
#include "MathGeoLib.h"
ComponentCamera::ComponentCamera()
{
	SetParent(nullptr);
	comp_type = ComponentType::camera;
	//Set frustum looking to GameObject Z axis
	

	//Position
	cam_frustum.pos = float3::zero;
	//Front
	cam_frustum.front = float3::unitZ;
	//Up
	cam_frustum.up = float3::unitY;
	// Near and Fear plane distance
	cam_frustum.nearPlaneDistance = 0.1;
	cam_frustum.farPlaneDistance= 1000.0f;

	// FOV and Aspect Ratio
	cam_frustum.horizontalFov = 90;
	cam_frustum.verticalFov = Atan(App->window->GetAspectRatio()* Tan(cam_frustum.verticalFov * 0.5f)) * 2.0f;
	float4x4 p = cam_frustum.ViewProjMatrix();
	//cam_frustum.SetVerticalFovAndAspectRatio(2 * math::Atan(math::Tan(cam_frustum.HorizontalFov()/ 2) * (1/App->window->GetAspectRatio())),App->window->GetAspectRatio());

}


ComponentCamera::ComponentCamera(GameObject* obj_parent)
{
	SetParent(obj_parent);
	comp_type = ComponentType::camera;
	//Set frustum looking to GameObject Z axis
	ComponentTransform* parent_transform = GetParent()->GetTransform();

	
	//Position
	cam_frustum.pos = parent_transform->translation;
	//Front
	cam_frustum.front = parent_transform->GetMatrix().Col3(2);
	//Up
	cam_frustum.up = parent_transform->GetMatrix().Col3(1);
	// Near and Fear plane distance
	cam_frustum.nearPlaneDistance = 0.1;
	cam_frustum.farPlaneDistance = 1000.0f;

	// FOV and Aspect Ratio
	cam_frustum.horizontalFov = 90;
	cam_frustum.verticalFov = Atan(App->window->GetAspectRatio()* Tan(cam_frustum.verticalFov * 0.5f)) * 2.0f;
	float4x4 p = cam_frustum.ViewProjMatrix();
	//cam_frustum.SetVerticalFovAndAspectRatio(2 * math::Atan(math::Tan(cam_frustum.HorizontalFov()/ 2) * (1/App->window->GetAspectRatio())),App->window->GetAspectRatio());
	
}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::UpdateComponent()
{
	
	ComponentTransform* parent_transform = GetParent()->GetTransform();
	cam_frustum.pos = parent_transform->translation;


	float3 col_2 = parent_transform->GetMatrix().Col3(2);
	float3 col_1 = parent_transform->GetMatrix().Col3(1);
	cam_frustum.front = col_2;
	cam_frustum.up = col_1;
	

	float color[4] = { 0.7f, 0.7f, 0.7f, 1 };
	cam_frustum.Draw(3.0f,color );

}

void ComponentCamera::ShowInspectorComponents()
{
	if (ImGui::CollapsingHeader("Camera", &header_open)) {

		//FOV
		float height = cam_frustum.verticalFov*RADTODEG;
		ImGui::DragFloat("Field of View", &height, 0.2f);
		cam_frustum.verticalFov = height*DEGTORAD;

		//ENABLE CULLING
		ImGui::Checkbox("Camera Culling", &frustum_culling);

		//PLANES DISTANCE
		float near_dist = cam_frustum.nearPlaneDistance;
		float far_dist = cam_frustum.farPlaneDistance;
		ImGui::DragFloat("Near Plane", &near_dist, 1.0f,0.0f,far_dist);
		ImGui::DragFloat("Far Plane", &far_dist, 1.0f, near_dist,5000.0f);
		cam_frustum.nearPlaneDistance = near_dist;
		cam_frustum.farPlaneDistance = far_dist;

	}

}



