#pragma once

#include "Module.h"
#include "Globals.h"



class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
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


	private:
		int begin = 0, end = 100;
		bool show_example_menu = false;
		bool show_menu = false;

		float rand = 0;
		float rand_2 = 0;
		LCG random_num_i;
		LCG random_num_f;
};