#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow( bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();

	bool CleanUp();

	update_status PreUpdate(float dt);

	void SetTitle(const char* title);

	void LoadConfig(JSON_Object*);

	void SaveConfig(JSON_Object*);

	bool Options();

public:

	//The window we'll be rendering to
	SDL_Window* window;
	//The surface contained by the window
	SDL_Surface* screen_surface;

	Uint32 flags;
	bool active = false;
	bool resizable = false;
	bool resizablebox = false;
	bool borderless = true;
	bool borderlessbox = false;
	bool fullScreen = false;
	bool fullScreenbox = false;
	int width;
	int height;
	const char* title;
};

#endif // __ModuleWindow_H__