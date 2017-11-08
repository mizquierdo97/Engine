#include "Component.h"
#include "ImGui\imgui.h"
#include "Application.h"

void Component::DeleteComponent()
{
	
}

void Component::UpdateComponent()
{

}

void Component::ShowInspectorComponents()
{


}

Resource * Component::GetResource() const
{
	return App->resources->Get(res_uuid);
}
