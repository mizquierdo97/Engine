#include "ComponentTransform.h"
#include "ImGui\imgui.h"
#include "Object.h"

ComponentTransform::ComponentTransform(aiVector3D pos, aiQuaternion rot, aiVector3D sc, Object* obj_parent){
	translation = float3(pos.x,pos.y,pos.z) ;
	rotation = Quat(rot.x, rot.y, rot.z, rot.w);
	scale = float3(sc.x, sc.y, sc.z);
	parent = obj_parent;
	comp_type = ComponentType::transform;
}

void ComponentTransform::UpdateComponent()
{
	if (parent->obj_parent != nullptr) {
		
		float4x4 parent_matrix = parent->obj_parent->GetTransform()->matrix;
		matrix = matrix*parent_matrix;
	}
	else {
		matrix = matrix* float4x4::identity;
	}
}

void ComponentTransform::ShowInspectorComponents()
{
	
	if (ImGui::CollapsingHeader("Transform", &header_open)) {
		ImGui::InputFloat("input float", &translation[0], -10000.0f, 100000.0f); ImGui::SameLine();
		ImGui::InputFloat("input float", &translation[1], -10000.0f, 100000.0f); ImGui::SameLine();
		ImGui::InputFloat("input float", &translation[2], -10000.0f, 100000.0f); 
	}

}

const float4x4 ComponentTransform::GetMatrix()
{
	return float4x4::FromTRS(translation, rotation, scale);
	
}
