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
	static int type = 0;
	static bool b_open = false;
	ResourceTexture* res = (ResourceTexture*)App->resources->Get(diffuse_tex);
	if (ImGui::CollapsingHeader("Material")) {
			
		shader->bind();
		if (ImGui::Button("Change Diffuse")) {			
			b_open = true;
			type = 0;
		}

		if (ImGui::Button("Change Normal")) {
			b_open = true;
			type = 1;
		}

		for (int i = 3; i < 8; i++) {

			std::string name = "Change Texture ";
			std::string var_name = "_texture";
			char num = i + '0';
			name += num;
			var_name += num;
			uint tex_id = glGetUniformLocation(shader->mProgramID, var_name.c_str());
			
			if (tex_id == -1)
				continue;
			if (ImGui::Button(name.c_str())) {

			}
		}

		
		if (res != nullptr) {
			Texture* actual_tex = res->res_tex;
			ImGui::Image((void*)actual_tex->GetTexture(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		}
		
		

		if (ImGui::Button("Change Shader")) {
			shader->UpdateShaderProgram("", "Assets/Red.frsh");
		}
		int shader_pos = 0;
		std::string shaders_names;
		for (int i = 0; i < App->shaders->shader_vect.size(); i++) {
			shaders_names += App->shaders->shader_vect[i]->shader_name;
			shaders_names += '\0';
		}
		if (ImGui::Combo("Inputs Mode", &shader_pos, shaders_names.c_str())) {
			shader = App->shaders->shader_vect[shader_pos];

		}



		static float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		ImGui::ColorPicker4("##picker", (float*)&color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

		static float time_dt = 0;
		time_dt += App->rtime.dt;

		/*GLint timeLoc = glGetUniformLocation(shader.mProgramID, "_time");
		glUniform1f(timeLoc, time_dt);*/
		GLint colorLoc = glGetUniformLocation(shader->mProgramID, "_color");
		glUniform4fv(colorLoc, 1, &color[0]);

		shader->unbind();

		static ShaderType type;
		if (ImGui::Button("Change Vertex")) {
			App->shaders->shader_change = true;
			App->shaders->set_editor_text = true;
			type = ShaderType::vertex_shader;
		}
		ImGui::SameLine();
		if (ImGui::Button("Change Fragment")) {
			App->shaders->shader_change = true;
			App->shaders->set_editor_text = true;
			type = ShaderType::pixel_shader;
		}
		if (App->shaders->shader_change) {
			App->shaders->UpdateShaderWindow(this, type);
		}

	}


	if (b_open) {
		ImGui::SetNextWindowPos(ImVec2(App->input->GetMousepositionX()-100, App->input->GetMousepositionY()), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
		ImGui::Begin("Textures", &b_open);
		App->gui->ShowTextureMenu(this, type);
		ImGui::End();
	}

}
