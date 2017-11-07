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
#include "ModuleResourceManager.h"
#include "FileSystem.h"
#include <list>

enum Gamestatus {

	PLAY,
	STOP,
	PAUSE,
	NEXT_FRAME,

};
class FileSystem;
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
	ModuleResourceManager* resources;
	Gamestatus gameStatus;

	Config* config;
	FileSystem* filesystem;
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

struct Rtime 
{
	Timer startTime;
	float frame_time;
	Timer timeSinceGameStart;
	float dt;
};

struct Gtime
{
	Timer gtime;
	float timeScale;
	float Gamedt;


};