
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"]




#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"

#include "SDL\include\SDL_opengl.h"




struct ExampleAppConsole;

ModuleGUI::ModuleGUI( bool start_enabled) : Module( start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	
	//ImGui_ImplSdlGL3_Init(App->window->window);
	ImGui_ImplSdlGL3_Init(App->window->window);
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureKeyboard = true;
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
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{

	//TEST
	static float vec4f_1[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
	static float vec4f_2[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
	bool b_col = false;

	App->renderer3D->test_sphere1->pos.x = vec4f_1[0];
	App->renderer3D->test_sphere2->pos.x = vec4f_2[0];
	if (App->renderer3D->test_sphere1->Intersects(*App->renderer3D->test_sphere2)) {
		b_col = true;
	}
	//

	//ImGui_ImplSdlGL2_NewFrame(App->window->window);
	//ImGui_ImplSdlGL2_NewFrame(App->window->window);
	ImGuiIO& io = ImGui::GetIO();
		
	


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Open menu")) { show_menu = !show_menu; }
			if (ImGui::MenuItem("Help")) { show_help = !show_help; }
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
	if (show_help)
	{
		if (ImGui::MenuItem("Gui Demo")) { show_example_menu = !show_example_menu; }
		
		if (ImGui::MenuItem("Documentation"))
			ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97/Engine/wiki", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Latest release"))
			ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97/Engine/releases", NULL, NULL, SW_SHOWNORMAL);

		if (ImGui::MenuItem("Report a bug"))
			ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97/Engine/issues", NULL, NULL, SW_SHOWNORMAL);

		
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
		if (ImGui::CollapsingHeader("GeoMath"))
		{			
			ImGui::SliderFloat("First Sphere X", vec4f_1, -2.0f, 2.0f);
			ImGui::SliderFloat("Second Sphere X", vec4f_2, -2.0f, 2.0f);
			ImGui::Checkbox("Colision?", &b_col);
			
		}
		

		if (ImGui::CollapsingHeader("Hardware"))
		{
			Det_Cache = SDL_GetCPUCacheLineSize();
			ImGui::Text("Cache: %i", Det_Cache);

			Det_CPU = SDL_GetCPUCount();
			ImGui::Text("CPU: %i", Det_CPU);

			Det_RAM = SDL_GetSystemRAM();
			ImGui::Text("RAM: %i Gb", Det_RAM);
		}
		
		ImGui::End();
	}
	
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	ImGui::Begin("Options");

	App->Options();
	std::list<Module*>::iterator item = App->list_modules.begin();
	while (item != App->list_modules.end()) {
		(*item)->Options();
		item++;
	}
	ImGui::End();

	static bool show_app_console = true;
	ShowConsole(&show_app_console);

	
	//ImGui::Render();
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





void ModuleGUI::ShowConsole(bool * p_open)
{

	
	console.Draw("Example: Console", p_open);

}