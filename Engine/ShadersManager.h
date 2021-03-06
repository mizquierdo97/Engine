#ifndef _SHADER_MANAGER
#define _SHADER_MANAGER
#include "Module.h"
#include "ShaderProgram.h"
#include "TextEditor.h"
#include "Shader.h"
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
	bool CreateShaderFromArray(char* vert_buffer, char* frag_buffer, std::string name);
	bool CreateDefaultShader();
	ShaderProgram Find(std::string name);
	void ShowVertexShadersFolder( char* file_type, TextEditor*);
	void CreateShaderWindow();
	void UpdateShaderWindow(ComponentMaterial* comp, ShaderType);
	char* GetShaderPath(uint ID);
	char* GetShaderText(std::string path);

	void LoadMaterials();
public:
	std::vector<ShaderProgram*> shader_vect;

	ShaderProgram default_shader;
	TextEditor vertex_editor;
	TextEditor fragment_editor;
	TextEditor update_shader_editor;
	bool shaders_window = false;
	bool shader_change = false;
	bool set_editor_text = false;
	bool load_shader[2] = { false,false };
	bool new_shader[2] = { false,false };
};


#endif // !_SHADER_MANAGER

