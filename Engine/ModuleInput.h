#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput( bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMousepositionX() const
	{
		return mouse_x;
	}

	int GetMousepositionY() const
	{
		return mouse_y;
	}

	int GetMousepositionZ() const
	{
		return mouse_z;
	}

	int GetMousepositionXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMousepositionYMotion() const
	{
		return mouse_y_motion;
	}

	bool Options();
	

public:

	char* dropped_filedir;
	bool file_dropped = false;
	bool active = false;
private:
	
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;

	
};