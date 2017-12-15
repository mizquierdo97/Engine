#include "Application.h"
#include "ComponentMaterial.h"
#include "ModuleResourceManager.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ShadersManager.h"
#include "ResourceShader.h"
#include "ResourceShaderProgram.h"
void ComponentMaterial::UpdateComponent()
{


}

void ComponentMaterial::SaveComponentScene(Data * data)
{
	UUID null_uuid; RPC_STATUS stat;
	UuidCreateNil(&null_uuid);

	data->CreateSection("Material");

	if (UuidCompare(&res_uuid, &null_uuid, &stat) != 0)
		data->AddString("Shader Path", App->resources->Get(res_uuid)->file);
	else
		data->AddString("Shader Path", "default.material");
	
	if(UuidCompare(&diffuse_tex, &null_uuid, &stat) != 0)
		data->AddString("Diffuse Path",App->resources->Get(diffuse_tex)->file);
	else
		data->AddString("Diffuse Path", "");
	if (UuidCompare(&normal_tex, &null_uuid, &stat) != 0)
		data->AddString("Normals Path", App->resources->Get(normal_tex)->file);
	else
		data->AddString("Normals Path", "");

	data->AddVector4("Color", shader->color);
	data->CloseSection();
}

void ComponentMaterial::ShowInspectorComponents()
{
	shader->bind();
	static int tex_num = 0;
	static bool b_open = false;
	ResourceTexture* res = (ResourceTexture*)App->resources->Get(diffuse_tex);
	if (ImGui::CollapsingHeader("Material")) {
			
		
		if (ImGui::Button("Change Diffuse")) {			
			b_open = true;
			tex_num = 0;
		}

		if (ImGui::Button("Change Normal")) {
			b_open = true;
			tex_num = 1;
		}

		for (int i = 3; i < 8; i++) {

			std::string name = "Change Texture ";
			std::string var_name = "_texture";
			char num = i + '0';
			name += num;
			var_name += num;
			int tex_id = glGetUniformLocation(shader->mProgramID, var_name.c_str());
			
			if (tex_id == -1)
				continue;
			if (ImGui::Button(name.c_str())) {
				b_open = true;
				tex_num = i-1;
				int x = 0;
			}
		}
		
		if (res != nullptr) {
			Texture* actual_tex = res->res_tex;
			ImGui::Image((void*)actual_tex->GetTexture(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		}		

		
		int shader_pos = 0;
		std::string shaders_names;
		for (int i = 0; i < App->shaders->shader_vect.size(); i++) {
			shaders_names += App->shaders->shader_vect[i]->shader_name;
			shaders_names += '\0';
		}
		if (ImGui::Combo("Inputs Mode", &shader_pos, shaders_names.c_str())) {
			shader = App->shaders->shader_vect[shader_pos];
			
			for (std::map<UUID, Resource*>::iterator it = App->resources->resources.begin(); it != App->resources->resources.end(); ++it){
				if ((*it).second != nullptr && (*it).second->type == Resource::shader_program) {
					ResourceShaderProgram* res = (ResourceShaderProgram*)(*it).second;
					
					if (res->res_shader_program != nullptr && res->res_shader_program->shader_name == shader->shader_name) {
						this->res_uuid = res->uuid;
						break;
					}
				}
			}
		}

		
		ImGui::ColorPicker4("##picker", (float*)&shader->color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

		
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

	//TODO CHANGE THIS, maybe in shader manager?
	GLint colorLoc = glGetUniformLocation(shader->mProgramID, "_color");
	glUniform4fv(colorLoc, 1, &shader->color[0]);

	shader->unbind();
	if (b_open) {
		ImGui::SetNextWindowPos(ImVec2(App->input->GetMousepositionX()-100, App->input->GetMousepositionY()), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
		ImGui::Begin("Textures", &b_open);
		App->gui->ShowTextureMenu(this, tex_num);
		ImGui::End();
	}

}
