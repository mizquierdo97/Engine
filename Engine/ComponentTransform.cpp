#include "ComponentTransform.h"
#include "ImGui\imgui.h"
#include "Object.h"

ComponentTransform::ComponentTransform(aiVector3D pos, aiQuaternion rot, aiVector3D sc, Object* obj_parent){
	translation = float3(pos.x,pos.y,pos.z) ;
	rotation = Quat(rot.x, rot.y, rot.z, rot.w);
	scale = float3(sc.x, sc.y, sc.z);
	parent = obj_parent;
	comp_type = ComponentType::transform;
	matrix = float4x4::FromTRS(translation, rotation, scale);
}

void ComponentTransform::UpdateComponent()
{
	if (parent->obj_parent != nullptr) {
		
		//float4x4 parent_matrix = parent->obj_parent->GetTransform()->matrix;
		
		//float4x4 temp_matrix = GetMatrix();

		matrix.Decompose(translation, rotation, scale);
		//parent_matrix.Decompose(parent_trans, parent_rot, parent_scale);

		
	}
	else {
		//matrix = GetMatrix();
		matrix.Decompose(translation, rotation, scale);
	}
	
}

void ComponentTransform::ShowInspectorComponents()
{
	
	if (ImGui::CollapsingHeader("Transform", &header_open)) {
		ImGui::DragFloat3("Position", &translation[0],0.2f);

		float3 euler = rotation.ToEulerXYZ();
		euler *= RADTODEG;
		ImGui::DragFloat3("Rotation", &euler[0], 0.2f);
		ImGui::DragFloat3("Scale", &scale[0], 0.2f,0.0001);
		euler *= DEGTORAD;
		rotation = rotation.FromEulerXYZ(euler.x,euler.y,euler.z);
	}

	if (scale.x <= 0) scale.x = 0.001; if (scale.y <= 0) scale.y = 0.001; if (scale.z <= 0) scale.z = 0.001;

		matrix = float4x4::FromTRS(translation, rotation, scale);
}

const float4x4 ComponentTransform::GetMatrix()
{
	if (parent->obj_parent != nullptr) {
		return float4x4::FromTRS(translation, rotation, scale)* parent->obj_parent->GetTransform()->GetMatrix();
	}
	else {
		return float4x4::FromTRS(translation, rotation, scale);
	}
	
}
