#include "Application.h"
#include "ComponentMaterial.h"

void ComponentMaterial::UpdateComponent()
{


}

void ComponentMaterial::SaveComponentScene(Data * data)
{
	data->CreateSection("Material");
	data->AddString("Mesh Path", obj_tex->texture_path);
	data->CloseSection();
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
		ImGui::SetNextWindowPos(ImVec2(App->input->GetMousepositionX()-100, App->input->GetMousepositionY()), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
		ImGui::Begin("Texture", &b_open);
		App->gui->ShowTextureMenu(this);
		ImGui::End();
	}

}
