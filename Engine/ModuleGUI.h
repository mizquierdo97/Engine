#pragma once

#include "Module.h"
#include "Globals.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "Console.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI( bool start_enabled = true);
	~ModuleGUI();
	bool Init();

	bool Start();

	//PreUpdate
	update_status PreUpdate(float dt);

	//Update
	update_status Update(float dt);

	//PostUpdate
	update_status PostUpdate(float dt);

	bool CleanUp();

	void HandleInput(SDL_Event* _event) {
		ImGui_ImplSdlGL3_ProcessEvent(_event);
		
	}
	void ApplicationOptions() {

	}
	void ShowConsole(bool* p_open);
	
	private:
		int begin = 0, end = 100;
		bool show_example_menu = false;
		bool show_menu = false;
		bool show_help = false;

		int Det_RAM;
		int Det_CPU;
		int Det_Cache;
		float rand = 0;
		float rand_2 = 0;
		LCG random_num_i;
		LCG random_num_f;

public:
	ExampleAppConsole console;
};