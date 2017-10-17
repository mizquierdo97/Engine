#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGUI.h"
#include "ModuleWorld.h"
#include "Config.h"
#include "ModuleAssimp.h"
#include <list>



class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGUI* gui;
	ModuleWorld* world;
	ModuleAssimp* assimp;

	Config* config;
private:

	Timer	ms_timer;
	float	dt;

public:
	std::list<Module*> list_modules;
	bool win_active = false;
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	
	bool Options();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;