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
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width;
	int height;
};

#endif // __ModuleWindow_H__