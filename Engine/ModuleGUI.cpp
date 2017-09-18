
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"


#include "Glew\include\glew.h"
#include "ImGui\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib  */  
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib  */ 



ModuleGUI::ModuleGUI(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	ImGuiIO& io = ImGui::GetIO();
	io.WantTextInput = true;
	io.IniFilename = "/Settings/imgui.ini";
	return true;
}

bool ModuleGUI::Start()
{

	return true;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	
	ImGui_ImplSdlGL3_NewFrame(App->window->window);


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Show example menu")) { show_example_menu = !show_example_menu; }
			if (ImGui::MenuItem("Open menu")) { show_menu = !show_menu; }
			if (ImGui::MenuItem("Close App"))
			{
				return UPDATE_STOP;
			}


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

	}

	if (show_example_menu)
	{
		ImGui::ShowTestWindow();
	}
	if (show_menu)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
		ImGui::Begin("Menu", (bool*)true,window_flags);

		ImGui::Text("Menu bar for Engines");
		if (ImGui::CollapsingHeader("Random"))
		{
			ImGui::DragIntRange2("Int Range", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f ", "Max: %.1f ");

			if (ImGui::Button("Int Randomize")) 
			{
				rand = random_num_i.Int(begin, end);
			}
			ImGui::SameLine(150);  ImGui::Text("%.0f", rand);

			if (ImGui::Button("Float Randomize")) 
			{
				rand_2 = random_num_f.Float();
			}
			ImGui::SameLine(150);  ImGui::Text("%.2f", rand_2);
		}
		ImGui::End();
	}

	ImGui::Render();
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	return false;
}
