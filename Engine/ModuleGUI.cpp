
#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "imgui_dock.h"
#include "SDL\include\SDL_opengl.h"
#include "Object.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

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
	io.Fonts->AddFontFromFileTTF("Fonts\\Roboto-Regular.ttf", 16);
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


	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.68f, 0.69f, 0.69f, 0.91f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.74f, 0.73f, 0.73f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.69f, 0.69f, 0.69f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.57f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.59f, 0.59f, 0.74f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.65f, 0.65f, 0.65f, 0.70f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.59f, 0.60f, 0.59f, 0.73f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.88f, 0.88f, 0.88f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.42f, 0.48f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.91f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.11f, 0.47f, 0.63f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.17f, 0.67f, 0.89f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.46f, 0.47f, 0.48f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.30f, 0.55f, 0.56f, 0.89f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.57f, 1.00f);
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
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.87f, 0.87f, 0.87f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.10f, 0.17f, 0.29f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.30f, 0.21f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.37f, 0.63f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.69f, 0.69f, 0.69f, 0.35f);
	
	return true;
}

bool ModuleGUI::Start()
{
	
	

	App->filesystem->ImportImage("png_icon.png");
	App->renderer3D->loadTextureFromFile("png_icon.png", &png_tex,false);
	App->renderer3D->loadTextureFromFile("fbx_icon.png", &fbx_tex,false);


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
		static bool show_about = false;
		
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
				if (ImGui::MenuItem("SaveScene")) {
					App->world->SaveScene();

				}
				if (ImGui::MenuItem("LoadScene")) {
					App->world->LoadScene();

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Hardware")) { show_hardware = !show_hardware; }
				if (ImGui::MenuItem("Window options")) { App->window->active = !App->window->active; }
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
		
				if (ImGui::MenuItem("About"))show_about = !show_about;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Performance"))
			{
				if (ImGui::MenuItem("Performance"))App->win_active = !App->win_active;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("GameObject")) {
					CreateObject();

				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (show_example_menu) { ImGui::ShowTestWindow();}
		
		if (show_hardware)
		{
			ShowHardware();
		}

		if (show_menu)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
			ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
			ImGui::Begin("Menu", &show_menu, window_flags);

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
		if (show_about)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
			ImGui::SetNextWindowSize(ImVec2(550, 680));
			if (ImGui::Begin("About", &show_about, window_flags)) {
				ImGui::Text("Video Game engine. \n\nDeveloped by:\n");
				if (ImGui::SmallButton("Miquel Izquierdo")) ShellExecuteA(NULL, "open", "https://github.com/mizquierdo97", NULL, NULL, SW_SHOWNORMAL);
				if (ImGui::SmallButton("Guillermo Pinto")) ShellExecuteA(NULL, "open", "https://github.com/Guille1406", NULL, NULL, SW_SHOWNORMAL);
				ImGui::Separator();
				ImGui::Text("Software Used: \n\n");
				ImGui::Bullet(); if(ImGui::SmallButton("SDL")) ShellExecuteA(NULL, "open", "https://www.libsdl.org/", NULL, NULL, SW_SHOWNORMAL);
				ImGui::Bullet(); if (ImGui::SmallButton("OpenGL")) ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOWNORMAL);
				ImGui::Bullet(); if (ImGui::SmallButton("ImGui")) ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
				ImGui::Bullet(); if (ImGui::SmallButton("Assimp")) ShellExecuteA(NULL, "open", "http://assimp.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
				ImGui::Bullet(); if (ImGui::SmallButton("DevIL")) ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
				ImGui::Bullet(); if (ImGui::SmallButton("MathGeoLIB")) ShellExecuteA(NULL, "open", "http://clb.demon.fi/MathGeoLib/nightly/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::End();
		}
			ImVec2 display_size = ImGui::GetIO().DisplaySize;
			ImGui::SetNextWindowSize(display_size);
			ImGui::SetNextWindowPos(ImVec2(0, 20));
			ImGui::Begin("PanelEditor", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
			
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

			return UPDATE_CONTINUE;
	
}

update_status ModuleGUI::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	ImGui::SaveDocks();
	path_list.clear();
	RELEASE(png_tex);
	RELEASE(fbx_tex);
	
	return false;
}


void ModuleGUI::Assets()
{
	
	if (ImGui::BeginDock("Assets", false, false)) {
		ImTextureID tex_id = App->renderer3D->tex;
		std::list<std::string>::iterator item = path_list.begin();
		int i = 0;
		while (item != path_list.end())
		{
			ImGui::PushID(i);
			int frame_padding = 1;

			std::string path = (*item);
			std::string file_extension = GetExtension(path);
			

			if (!strcmp((char*)file_extension.c_str(), "bin")) {
				if (ImGui::ImageButton((void*)fbx_tex->GetTexture(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), frame_padding))
					App->filesystem->mesh_importer->LoadMesh(path.c_str());
			}


			else if (!strcmp((char*)file_extension.c_str(), "dds") || !strcmp((char*)file_extension.c_str(), "jpg") || !strcmp((char*)file_extension.c_str(), "png"))
			{
				if (ImGui::ImageButton((void*)png_tex->GetTexture(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), frame_padding)) {
					std::vector<GameObject*>::iterator item = App->world->obj_vector.begin();
					for (; item != App->world->obj_vector.end(); item++)
						App->renderer3D->loadTextureFromFile("png_icon.png", &(*item)->GetMaterial()->obj_tex);
				}
			}

			ImGui::PopID();
			ImGui::SameLine();
			i++; item++;
		}
		ImGui::EndDock();
	}
	if (ImGui::BeginDock("Inspector", false, false)) {


		GameObject* object = App->world->GetSelectedObject();

		if (object != nullptr) {
			std::vector<Component*>::iterator item = object->obj_components.begin();
			for (; item != object->obj_components.end(); item++)
			{

				switch ((*item)->comp_type) {
				case material: {
					ComponentMaterial* temp_mat = (ComponentMaterial*)(*item);
					temp_mat->ShowInspectorComponents();
					break;
				}
				case mesh: {
					ComponentMesh* temp_mesh = (ComponentMesh*)(*item);
					temp_mesh->ShowInspectorComponents();
					break;
				}

				case transform: {
					ComponentTransform* temp_trans = (ComponentTransform*)(*item);
					temp_trans->ShowInspectorComponents();
					break;
				}
				case camera: {
					ComponentCamera* temp_cam = (ComponentCamera*)(*item);
					temp_cam->ShowInspectorComponents();
					break;
				}
				}

			}
		}


		ImGui::Separator();

		if (ImGui::Button("Add Component", ImVec2(400, 30))&&App->world->IsObjectSelected())
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			ShowAddComponent();
			ImGui::EndPopup();
		}

		ImGui::EndDock();
	}



}


 void ModuleGUI::ShowAddComponent()
{
	 if (ImGui::MenuItem("Transformation"))
	 {
		 App->world->GetSelectedObject()->AddComponentTransform();
	 }

	if (ImGui::MenuItem("Mesh")) 
	{

		//Selected->AddComponentMesh(Mesh);
	}
	if (ImGui::MenuItem("Material")) 
	{
		App->world->GetSelectedObject()->AddComponentMaterial(App->world->world_texture);
	}
	if (ImGui::MenuItem("Camera"))
	{
		App->world->GetSelectedObject()->AddComponentCamera();
	}
	

	
	
}
 void ModuleGUI::ShowTextureMenu(ComponentMaterial * comp)
 {
	 std::list<std::string>::iterator item = path_list.begin();
	 int i = 0;
	 while (item != path_list.end())
	 {
		 ImGui::PushID(i);
		 int frame_padding = 1;

		 std::string path = (*item);
		 std::string file_extension = GetExtension(path);	

		 if (!strcmp((char*)file_extension.c_str(), "dds") || !strcmp((char*)file_extension.c_str(), "jpg") || !strcmp((char*)file_extension.c_str(), "png"))
		 {
			 if (ImGui::ImageButton((void*)png_tex->GetTexture(), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), frame_padding)) {
				 std::vector<GameObject*>::iterator item = App->world->obj_vector.begin();
				 App->renderer3D->loadTextureFromFile("png_icon.png", &comp->obj_tex);

			 }
		 }
			 ImGui::PopID();
			 ImGui::SameLine();
			 i++; item++;

	 }

 }


void ModuleGUI::ShowConsole(bool * p_open)
{
		
	console.Draw("Example: Console", p_open);

}

void ModuleGUI::ShowHardware() 
{
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Hardware", &show_hardware);

	int major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MAJOR_VERSION, &minor);
	ImGui::Text("OpenGL Version:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i.%i", major, minor);
	ImGui::Text("GPU Vendor:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, (const char*)glGetString(GL_VENDOR));
	ImGui::Text("Model:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, (const char*)glGetString(GL_RENDERER));
	ImGui::Text("Total VRAM:");
	GLint total_mem_kb = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
		&total_mem_kb);
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i MB", total_mem_kb / 1024);

	ImGui::Text("Free VRAM:");
	GLint cur_avail_mem_kb = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
		&cur_avail_mem_kb);
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i MB", cur_avail_mem_kb / 1024);
	Det_Cache = SDL_GetCPUCacheLineSize();
	ImGui::Text("Cache:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i MB", Det_Cache);

	Det_CPU = SDL_GetCPUCount();
	ImGui::Text("Number of CPU:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i", Det_CPU);

	Det_RAM = SDL_GetSystemRAM();
	ImGui::Text("RAM Memory:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i Gb", Det_RAM);

	SDL_GetVersion(&sdl_vers);
	ImGui::Text("SDL Version:");
	ImGui::Text("Patch:"); ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i", sdl_vers.patch);
	ImGui::SameLine();
	ImGui::Text("Major:"); ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i", sdl_vers.major);
	ImGui::SameLine();
	ImGui::Text("Minor:"); ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i", sdl_vers.minor);
	ImGui::SameLine();

	ImGui::Text("Developer's Image Library (DevIL): version "); ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "1.7.8");

	ImGui::Separator();
	ImGui::Text("Gamepads Connected:");
	ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i", SDL_NumJoysticks());
	ImGui::End();
	ImGui::End();

}

