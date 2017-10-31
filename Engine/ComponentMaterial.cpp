#include "Application.h"
#include "ComponentMaterial.h"

void ComponentMaterial::UpdateComponent()
{


}

void ComponentMaterial::ShowInspectorComponents()
{
	static bool b_open = false;
	if (ImGui::CollapsingHeader("Material", &header_open)) {
		
		if (ImGui::Button("Texture")) {			
			b_open = true;
		}
	}

	if (b_open) {
		ImGui::SetNextWindowPos(ImVec2(App->input->GetMouseX()-100, App->input->GetMouseY()), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
		ImGui::Begin("Texture", &b_open);
		App->gui->ShowTextureMenu(this);
		ImGui::End();
	}

}
