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
	
		bool show_example_menu = false;
	
};