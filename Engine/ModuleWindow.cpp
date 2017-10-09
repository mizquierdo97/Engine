#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui_dock.h"
ModuleWindow::ModuleWindow( bool start_enabled) : Module( start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
	
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

update_status ModuleWindow::PreUpdate(float dt)
{
	
	SDL_GetWindowSize(window,&width,&height);
	return UPDATE_CONTINUE;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::LoadConfig(JSON_Object * root)
{
	
	JSON_Value* value;
	
	if (json_object_get_value(root, "Width") == NULL) {
		json_object_set_value(root, "Width", json_value_init_object());
		width = SCREEN_WIDTH * SCREEN_SIZE;
		json_object_set_number(root, "Width", width);
	
	}
	else {
		width = json_object_get_number(root, "Width")  * SCREEN_SIZE;

	}

	if (json_object_get_value(root, "Height") == NULL) {
		json_object_set_value(root, "Height", json_value_init_object());
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		json_object_set_number(root, "Height", height);
	}
	else {
		height = json_object_get_number(root, "Height") * SCREEN_SIZE;

	}

	if (json_object_get_value(root, "Window Name") == NULL) {
		json_object_set_value(root, "Window Name", json_value_init_object());
		title = TITLE;
		json_object_set_string(root, "Window Name", title);
	}
	else {
		
		title = json_object_get_string(root, "Window Name");

	}

}

void ModuleWindow::SaveConfig(JSON_Object * root)
{

	json_object_set_number(root, "Width", width / SCREEN_SIZE);
	json_object_set_number(root, "Height", height / SCREEN_SIZE);
	json_object_set_string(root, "Window Name",title);
}

bool ModuleWindow::Options()
{

	if (ImGui::BeginDock("Window", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar )) {
		ImGui::Columns(2);
	
		static int temp_width = width;
		static int temp_height = height;
		ImGui::SliderInt("Width", &temp_width, 300, 1400);
		ImGui::SliderInt("Height", &temp_height, 300, 1400);
		if (ImGui::Button("Save")) {
			width = temp_width;
			height = temp_height;
			SDL_SetWindowSize(window, width, height);
		}
		ImGui::NextColumn();
			if (ImGui::Checkbox("Borderless", &borderlessbox))
			{
				if (borderless == false)
				{
					flags -= SDL_WINDOW_BORDERLESS;
					borderless = !borderless;
					SDL_SetWindowBordered(window, (SDL_bool)borderless);
				}

				else
				{
					flags |= SDL_WINDOW_BORDERLESS;
					borderless = !borderless;
					SDL_SetWindowBordered(window, (SDL_bool)borderless);
				}

			}

			if (ImGui::Checkbox("FullScreen", &fullScreenbox))
			{

				
				if (fullScreen == true) {

					flags -= SDL_WINDOW_FULLSCREEN;
					fullScreen = !fullScreen;
					SDL_SetWindowFullscreen(window, flags);
				}

				else {
					flags |= SDL_WINDOW_FULLSCREEN;
					SDL_SetWindowFullscreen(window, flags);
					fullScreen = !fullScreen;
				}
			}
			

		
		//---------------------
		static char name[64] = "";
		strcpy(name, title);
		if (ImGui::InputText("Project Title", name, IM_ARRAYSIZE(name)))
			SetTitle(name);
		title = name;

		ImGui::EndDock();
	}
	return false;
}
