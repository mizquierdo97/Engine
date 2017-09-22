#pragma once

#include "Application.h"

#include "Globals.h"
#include "Json\parson.h"


class ModuleConfig: public Module
{
public:
	ModuleConfig(bool start_enabled = true);
	~ModuleConfig();
	bool Init();

	bool Start();

	bool CleanUp();


};