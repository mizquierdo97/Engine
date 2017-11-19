#include "Application.h"
#include "ComponentMaterial.h"
#include "ModuleResourceManager.h"
#include "ResourceTexture.h"
void ComponentMaterial::UpdateComponent()
{


}

void ComponentMaterial::SaveComponentScene(Data * data)
{
	data->CreateSection("Material");
	data->AddString("Texture Path", ((ResourceTexture*)App->resources->Get(res_uuid))->file);
	data->CloseSection();
}

void ComponentMaterial::ShowInspectorComponents()
{
	static bool b_open = false;
	if (ImGui::CollapsingHeader("Material")) {
		
		if (ImGui::Button("Change Texture")) {			
			b_open = true;
		}
		if (GetResource() != nullptr) {
			Texture* actual_tex = ((ResourceTexture*)GetResource())->res_tex;
			ImGui::Image((void*)actual_tex->GetTexture(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		}
	}

	if (b_open) {
		ImGui::SetNextWindowPos(ImVec2(App->input->GetMousepositionX()-100, App->input->GetMousepositionY()), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
		ImGui::Begin("Textures", &b_open);
		App->gui->ShowTextureMenu(this);
		ImGui::End();
	}

}
