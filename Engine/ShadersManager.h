#ifndef _SHADER_MANAGER
#define _sHADER_MANAGER
#include "Module.h"

class Application;

class ShadersManager:public Module {

public:

	ShadersManager(bool start_enabled = true);

	// Destructor
	virtual ~ShadersManager() {

	};
/*
	bool Init();
	bool Start();
	bool CleanUp();
	
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	*/


};


#endif // !_SHADER_MANAGER

