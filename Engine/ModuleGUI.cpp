
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"]




#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "imgui_dock.h"
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
	io.Fonts->AddFontFromFileTTF("..\\Game\\Fonts\\Roboto-Regular.ttf", 16);
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 8.0f;
	style.FramePadding = { 7.0f, 3.0f };
	style.FrameRounding = 66.0f;
	style.ItemSpacing = { 9.0f,6.0f };
	style.ScrollbarSize = 20.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 8.0f;
	style.WindowTitleAlign = { 0.50f,0.50f };


	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.65f, 0.61f, 0.61f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.46f, 0.46f, 0.46f, 0.80f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.19f, 0.19f, 0.19f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.50f, 0.49f, 0.49f, 0.57f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.74f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.63f, 0.69f, 0.70f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.37f, 0.79f, 0.86f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.34f, 0.71f, 0.77f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.38f, 0.39f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.91f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.11f, 0.47f, 0.63f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.17f, 0.67f, 0.89f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.46f, 0.47f, 0.48f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.45f, 0.54f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.31f, 0.72f, 0.77f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.34f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.46f, 0.73f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.47f, 0.69f, 0.56f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.05f, 0.22f, 0.36f, 0.90f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.19f, 0.28f, 0.80f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.10f, 0.17f, 0.29f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);



	return true;
}

bool ModuleGUI::Start()
{
	App->renderer3D->loadTextureFromFile("png_icon.png", &png_tex);
	App->renderer3D->loadTextureFromFile("fbx_icon.png", &fbx_tex);
	
	path_list = new std::list<std::string>;
	ImGui::LoadDocks();
	return true;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{

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
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open menu")) { show_menu = !show_menu; }

				if (ImGui::MenuItem("Close App"))
				{
					return UPDATE_STOP;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Hardware")) { show_hardware = !show_hardware; }

				ImGui::EndMenu();
			}

			

			if (ImGui::BeginMenu("Help"))
			{

				if (ImGui::MenuItem("GUI Demo")) { show_example_menu = !show_example_menu; }

				if (ImGui::MenuItem("Documentation"))
					ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97/Engine/wiki", NULL, NULL, SW_SHOWNORMAL);

				if (ImGui::MenuItem("Latest release"))
					ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97/Engine/releases", NULL, NULL, SW_SHOWNORMAL);

				if (ImGui::MenuItem("Report a bug"))
					ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97/Engine/issues", NULL, NULL, SW_SHOWNORMAL);

				if (ImGui::MenuItem("About")) {}


				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();

		}

		if (show_example_menu) { ImGui::ShowTestWindow();}
		

		if (show_menu)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
			ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
			ImGui::Begin("Menu", (bool*)true, window_flags);

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

		ImVec2 display_size = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowSize(display_size);
		ImGui::SetNextWindowPos(ImVec2(0, 20));
		ImGui::Begin("PanelEditor", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar );


		ImGui::Separator();
		ImGui::BeginDockspace();

		App->Options();
		Assets();
		std::list<Module*>::iterator item = App->list_modules.begin();
		while (item != App->list_modules.end()) {
			(*item)->Options();
			item++;
		}

		static bool show_app_console = true;
		ShowConsole(&show_app_console);
		ImGui::EndDockspace();
		ImGui::End();
	
	


		//ImGui::Render();
		return UPDATE_CONTINUE;
	
}

update_status ModuleGUI::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	ImGui::SaveDocks();
	return false;
}





void ModuleGUI::Assets()
{
	if (ImGui::BeginDock("Assets", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImTextureID tex_id = App->renderer3D->tex;
		std::list<std::string>::iterator item = path_list->begin();
		int i = 0;
		while(item != path_list->end())
		{
			ImGui::PushID(i);
			int frame_padding =1;     // -1 = uses default padding

			std::string path =(*item);
			std::string temp;
			temp = path.substr(path.length() - 3, 3);

			if (!strcmp((char*)temp.c_str(), "fbx")) {
				if (ImGui::ImageButton((void*)fbx_tex->GetTexture(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), frame_padding))
					App->world->ImportMesh((char*)path.c_str());
			}
				
			//ImportMesh(App->input->dropped_filedir);
			else if (!strcmp((char*)temp.c_str(), "png"))
			{
				if (ImGui::ImageButton((void*)png_tex->GetTexture(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), frame_padding))
					App->renderer3D->loadTextureFromFile((char*)path.c_str(), &App->renderer3D->tex);
			}
				
			//
			
				
			ImGui::PopID();
			ImGui::SameLine();
			i++; item++;
		}
	}
	ImGui::EndDock();
}

void ModuleGUI::ShowConsole(bool * p_open)
{
		
	console.Draw("Example: Console", p_open);

}