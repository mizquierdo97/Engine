#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager(bool start_enabled = true);

	// Destructor
	virtual ~ModuleResourceManager() {};

	bool Init();

	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);



	//	void LoadConfig(JSON_Object*);

	//	void SaveConfig(JSON_Object*);


};