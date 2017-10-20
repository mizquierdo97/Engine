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
		matrix = GetMatrix()*parent_matrix;

		matrix.Decompose(translation, rotation, scale);
	}

}

void ComponentTransform::ShowInspectorComponents()
{
	
	if (ImGui::CollapsingHeader("Transform", &header_open)) {
		ImGui::DragFloat3("Pos", &translation[0]);
	}

}

const float4x4 ComponentTransform::GetMatrix()
{
	return float4x4::FromTRS(translation, rotation, scale);
	
}
