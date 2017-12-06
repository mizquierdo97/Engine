#include "Shader_Editor.h"
#include"Application.h"

Shader_Editor::Shader_Editor()
{
}

Shader_Editor::~Shader_Editor()
{
}

void Shader_Editor::Enable(bool shaders_window) {
	
	
	

		float2 next_win_size = float2(500, 600);
		ImGui::SetNextWindowPos(ImVec2((App->window->width / 2) - next_win_size.x / 2, (App->window->height / 2) - next_win_size.y / 2));
		ImGui::SetNextWindowSize(ImVec2(next_win_size.x, next_win_size.y));

		if (ImGui::Begin("Shaders", &shaders_window)) {

			
			ImGui::Separator();

		

			if (!App->shaders->vertex_shader_window) {
				if (ImGui::Button("Load")) {
					App->shaders->vertex_shader_window = true;
			
					text_shader_editor.InsertText("//This is meant to be an editor of shaders");
					text_shader_editor.InsertText("\n");
				}
				
				ImGui::SameLine();
				ImGui::Button("New");
				if (App->shaders->vertex_shader_window) {
					App->shaders->ShowVertexShadersFolder();
				}

			}
			else {
				App->shaders->ShowVertexShadersFolder();
			}

			text_shader_editor.Render("Test");
		}
		ImGui::End();
}