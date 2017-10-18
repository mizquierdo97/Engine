#include "ComponentTransform.h"
#include "ImGui\imgui.h"
ComponentTransform::ComponentTransform(aiVector3D pos, aiQuaternion rot, aiVector3D sc){
	translation = float3(pos.x,pos.y,pos.z) ;
	rotation = Quat(rot.x, rot.y, rot.z, rot.w);
	scale = float3(sc.x, sc.y, sc.z);
	comp_type = ComponentType::transform;
}

void ComponentTransform::UpdateComponent()
{

}

void ComponentTransform::ShowInspectorComponents()
{
	
	if (ImGui::CollapsingHeader("Transform", &header_open)) {
		ImGui::InputFloat("input float", &translation[0], -10000.0f, 100000.0f); ImGui::SameLine();
		ImGui::InputFloat("input float", &translation[1], -10000.0f, 100000.0f); ImGui::SameLine();
		ImGui::InputFloat("input float", &translation[2], -10000.0f, 100000.0f); 
	}

}
