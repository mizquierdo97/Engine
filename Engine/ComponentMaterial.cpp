#include "Application.h"
#include "ComponentMaterial.h"
#include "ModuleResourceManager.h"
#include "ResourceTexture.h"
#include "ShadersManager.h"
#include "ResourceShader.h"
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
	ResourceTexture* res = (ResourceTexture*)GetResource();
	if (ImGui::CollapsingHeader("Material")) {
				
		if (ImGui::Button("Change Texture")) {			
			b_open = true;
		}
		if (res != nullptr) {
			Texture* actual_tex = res->res_tex;
			ImGui::Image((void*)actual_tex->GetTexture(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		}
		if (ImGui::CollapsingHeader("Shader")) {
			if (ImGui::Button("Change Shader")) {
				shader.UpdateShader("", "Assets/Red.frsh");
			}
			shader.bind();

			static float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
			
			ImGui::ColorPicker4("##picker", (float*)&color,  ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			
			GLint colorLoc = glGetUniformLocation(shader.mProgramID, "myColor");
			glUniform4fv(colorLoc, 1, &color[0]);

			shader.unbind();



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
