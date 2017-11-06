#include "ComponentTransform.h"
#include "ImGui\imgui.h"
#include "Object.h"

ComponentTransform::ComponentTransform(aiVector3D pos, aiQuaternion rot, aiVector3D sc, GameObject* obj_parent){
	translation = float3(pos.x,pos.y,pos.z) ;
	rotation = Quat(rot.x, rot.y, rot.z, rot.w);
	scale = float3(sc.x, sc.y, sc.z);
	SetParent(obj_parent);
	comp_type = ComponentType::transform;
	matrix = float4x4::FromTRS(translation, rotation, scale);
}

ComponentTransform::ComponentTransform(float3 pos, float4 rot, float3 sc, GameObject* obj_parent) {
	translation = float3(pos.x, pos.y, pos.z);
	rotation = Quat(rot.x, rot.y, rot.z, rot.w);
	scale = float3(sc.x, sc.y, sc.z);
	SetParent(obj_parent);
	comp_type = ComponentType::transform;
	matrix = GetMatrix();
}


void ComponentTransform::UpdateComponent()
{
	matrix.Decompose(translation, rotation, scale);
}

void ComponentTransform::ShowInspectorComponents()
{
	
	if (ImGui::CollapsingHeader("Transform", &header_open)) {
		ImGui::DragFloat3("Position", &translation[0],0.2f);

		float3 before_scale = scale;
		float3 euler = rotation.ToEulerXYZ();
		euler *= RADTODEG;
		ImGui::DragFloat3("Rotation", &euler[0], 0.2f);
		ImGui::DragFloat3("Scale", &scale[0], 0.2f,0.0001);
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &lock_scale_prop);		
		euler *= DEGTORAD;
		rotation = rotation.FromEulerXYZ(euler.x,euler.y,euler.z);

		if (lock_scale_prop)
			LockProprtionScale(before_scale);
	}

	if (scale.x <= 0) scale.x = 0.001; if (scale.y <= 0) scale.y = 0.001; if (scale.z <= 0) scale.z = 0.001;

		matrix = float4x4::FromTRS(translation, rotation, scale);
}

void ComponentTransform::LockProprtionScale(float3 before_scale)
{
	float prop = 1.0f;
	if (before_scale.x != scale.x) {
		prop = before_scale.x / scale.x;
		scale.y /= prop;
		scale.z /= prop;
	}
	else if (before_scale.y != scale.y) {
		prop = before_scale.y / scale.y;
		scale.x /= prop;
		scale.z /= prop;
	}
	else if (before_scale.z != scale.z) {
		prop = before_scale.z / scale.z;
		scale.y /= prop;
		scale.x /= prop;
	}

}

const float4x4 ComponentTransform::GetMatrix()
{
	if (GetParent()->obj_parent != nullptr) {
		return  GetParent()->obj_parent->GetTransform()->GetMatrix()* GetMyMatrix();
	}
	else {
		return GetMyMatrix();
	}
	
}
const float4x4 ComponentTransform::GetMyMatrix()
{
	return float4x4::FromTRS(translation, rotation, scale);
}