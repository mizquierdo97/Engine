#include "ComponentCamera.h"
#include "Application.h"


ComponentCamera::ComponentCamera(GameObject* obj_parent)
{
	SetParent(obj_parent);
	comp_type = ComponentType::camera;
	//Set frustum looking to GameObject Z axis
	ComponentTransform* parent_transform = GetParent()->GetTransform();

	
	//Position
	cam_frustum.SetPos(parent_transform->translation);
	//Front
	cam_frustum.SetFront(parent_transform->GetMatrix().Col3(2));
	//Up
	cam_frustum.SetUp(parent_transform->GetMatrix().Col3(1));
	// Near and Fear plane distance
	cam_frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	// FOV and Aspect Ratio
	cam_frustum.SetHorizontalFovAndAspectRatio(90 * DEGTORAD, App->window->GetAspectRatio());
	//cam_frustum.SetVerticalFovAndAspectRatio(2 * math::Atan(math::Tan(cam_frustum.HorizontalFov()/ 2) * (1/App->window->GetAspectRatio())),App->window->GetAspectRatio());
	
}


ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::UpdateComponent()
{
	
	ComponentTransform* parent_transform = GetParent()->GetTransform();
	cam_frustum.SetPos(parent_transform->translation);


	float3 col_2 = parent_transform->GetMatrix().Col3(2);
	float3 col_1 = parent_transform->GetMatrix().Col3(1);
	cam_frustum.SetFront(col_2);
	cam_frustum.SetUp(col_1);
	

	float color[4] = { 0.7f, 0.7f, 0.7f, 1 };
	cam_frustum.Draw(3.0f,color );

}

void ComponentCamera::ShowInspectorComponents()
{
	if (ImGui::CollapsingHeader("Camera", &header_open)) {

		//FOV
		float height = cam_frustum.VerticalFov()*RADTODEG;
		ImGui::DragFloat("Field of View", &height, 0.2f);
		cam_frustum.SetVerticalFovAndAspectRatio(height*DEGTORAD, cam_frustum.AspectRatio());

		//ENABLE CULLING
		ImGui::Checkbox("Camera Culling", &frustum_culling);

		//PLANES DISTANCE
		float near_dist = cam_frustum.NearPlaneDistance();
		float far_dist = cam_frustum.FarPlaneDistance();
		ImGui::DragFloat("Near Plane", &near_dist, 1.0f,0.0f,far_dist);
		ImGui::DragFloat("Far Plane", &far_dist, 1.0f, near_dist,5000.0f);
		cam_frustum.SetViewPlaneDistances(near_dist, far_dist);

	}

}

void ComponentCamera::Look(const float3 & position)
{
	float3 Direction = position - cam_frustum.Pos;

	float3x3 lookAt = float3x3::LookAt(cam_frustum.Front, Direction.Normalized(), cam_frustum.Up, float3::unitY);

	cam_frustum.Front = lookAt.MulDir(cam_frustum.Front).Normalized();
	cam_frustum.Up = lookAt.MulDir(cam_frustum.Up).Normalized();
}


