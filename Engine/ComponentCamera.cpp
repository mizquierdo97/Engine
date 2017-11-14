#include "ComponentCamera.h"
#include "Application.h"
#include "MathGeoLib.h"


ComponentCamera::ComponentCamera()
{
	SetParent(nullptr);
	comp_type = ComponentType::camera;
	//Set frustum looking to GameObject Z axis
	

	//Position
	cam_frustum.type = FrustumType::PerspectiveFrustum;
	cam_frustum.pos = float3::zero;
	//Front
	cam_frustum.front = float3(0.0f,0.0f,1.0f);
	//Up
	cam_frustum.up = float3(0.0f, 1.0f, 0.0f);
	// Near and Fear plane distance
	cam_frustum.nearPlaneDistance = 0.1;
	cam_frustum.farPlaneDistance= 1000.0f;

	// FOV and Aspect Ratio
	cam_frustum.verticalFov = 60* DEGTORAD;
	cam_frustum.horizontalFov = math::Atan(App->window->GetAspectRatio()*math::Tan(cam_frustum.verticalFov / 2)) * 2;
	float4x4 p = cam_frustum.ViewProjMatrix();
	

}


ComponentCamera::ComponentCamera(GameObject* obj_parent)
{
	SetParent(obj_parent);
	
	comp_type = ComponentType::camera;
	//Set frustum looking to GameObject Z axis


	//Position
	cam_frustum.type = FrustumType::PerspectiveFrustum;
	cam_frustum.pos = float3::zero;
	//Front
	cam_frustum.front = float3(0.0f, 0.0f, 1.0f);
	//Up
	cam_frustum.up = float3(0.0f, 1.0f, 0.0f);
	// Near and Fear plane distance
	cam_frustum.nearPlaneDistance = 0.1;
	cam_frustum.farPlaneDistance = 1000.0f;

	// FOV and Aspect Ratio
	cam_frustum.verticalFov = 60 * DEGTORAD;
	cam_frustum.horizontalFov = math::Atan(App->window->GetAspectRatio()*math::Tan(cam_frustum.verticalFov / 2)) * 2;
	float4x4 p = cam_frustum.ViewProjMatrix();
}

ComponentCamera::ComponentCamera(GameObject * obj_parent, Frustum frus)
{
	SetParent(obj_parent);
	comp_type = ComponentType::camera;
	cam_frustum.type = FrustumType::PerspectiveFrustum;
	cam_frustum.pos = frus.pos;
	//Front
	cam_frustum.front = frus.front;
	//Up
	cam_frustum.up = frus.up;
	// Near and Fear plane distance
	cam_frustum.nearPlaneDistance = frus.nearPlaneDistance;
	cam_frustum.farPlaneDistance = frus.farPlaneDistance;

	// FOV and Aspect Ratio
	cam_frustum.verticalFov = frus.verticalFov;
	cam_frustum.horizontalFov = frus.horizontalFov;
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

void ComponentCamera::SaveComponentScene(Data * data)
{
	data->CreateSection("Camera");

	data->AddVector3("Position",cam_frustum.pos);	
	data->AddVector3("Front", cam_frustum.front);
	data->AddVector3("Up", cam_frustum.up);
	data->AddFloat("Near", cam_frustum.nearPlaneDistance);
	data->AddFloat("Far", cam_frustum.farPlaneDistance);
	data->AddFloat("Vertical FOV", cam_frustum.verticalFov);
	data->AddFloat("Horizontal FOV", cam_frustum.horizontalFov);
	
	data->CloseSection();

}

void ComponentCamera::ShowInspectorComponents()
{
	if (ImGui::CollapsingHeader("Camera", &header_open)) {

		//FOV
		float height = cam_frustum.verticalFov*RADTODEG;		
		ImGui::DragFloat("Field of View", &height, 0.2f, 0.1f, 179.0f);
		cam_frustum.verticalFov = height*DEGTORAD;
		cam_frustum.horizontalFov = math::Atan(App->window->GetAspectRatio()*math::Tan(cam_frustum.verticalFov / 2)) * 2;

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



