#ifndef _SHADER_MANAGER
#define _SHADER_MANAGER
#include "Module.h"
#include "ShaderProgram.h"
#include <vector>
class Application;

class ShadersManager:public Module {

public:

	ShadersManager(bool start_enabled = true);

	// Destructor
	virtual ~ShadersManager() {

	};



	bool Init();
	bool Start();
	bool CleanUp();
	
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	
	void CreateShader(std::string vs_path, std::string fs_path, std::string name);
	bool CreateDefaultShader();
	ShaderProgram Find(std::string name);
public:
	std::vector<ShaderProgram*> shader_list;

	ShaderProgram default_shader;
};


#endif // !_SHADER_MANAGER

