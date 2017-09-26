#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"


class ModuleWorld : public Module
{
public: 

	ModuleWorld(bool start_enable = true);
	~ModuleWorld();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:



};
