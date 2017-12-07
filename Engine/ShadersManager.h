#ifndef _SHADER_MANAGER
#define _SHADER_MANAGER
#include "Module.h"
#include "ShaderProgram.h"
#include "TextEditor.h"
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
	void CreateShaderFromArray(char* vert_buffer, char* frag_buffer, std::string name);
	bool CreateDefaultShader();
	ShaderProgram Find(std::string name);
	void ShowVertexShadersFolder( char* file_type, TextEditor*);
	void CreateShaderWindow();
	char* GetShaderText(std::string path);
public:
	std::vector<ShaderProgram*> shader_vect;

	ShaderProgram default_shader;
	TextEditor vertex_editor;
	TextEditor fragment_editor;
	bool shaders_window = false;
	bool vertex_shader_window[2] = { false,false };
};


#endif // !_SHADER_MANAGER

