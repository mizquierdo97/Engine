#include "Application.h"
#include "Resource.h"
#include "ResourceShader.h"
#include "ShadersManager.h"
#include "ComponentMaterial.h"
#include "FileSystem.h"

ShadersManager::ShadersManager(bool start_enabled) : Module(start_enabled)
{

}

bool ShadersManager::Init()
{
	return true;
}

bool ShadersManager::Start()
{
	CreateDefaultShader();
	LoadMaterials();
	
	return true;
}

bool ShadersManager::CleanUp()
{
	return true;
}

update_status ShadersManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ShadersManager::Update(float dt)
{
	std::vector<ShaderProgram*>::iterator item = shader_vect.begin();
	static float time_dt = 0;
	time_dt += dt*App->gtime.timeScale;
	while (item != shader_vect.end()) {

		(*item)->bind();

		//TIME		
		GLint timeLoc = glGetUniformLocation((*item)->mProgramID, "_time");
		glUniform1f(timeLoc, time_dt);

		//COLOR
		GLint colorLoc = glGetUniformLocation((*item)->mProgramID, "_color");
		glUniform4fv(colorLoc, 1, &(*item)->color[0]);

		(*item)->unbind();

		item++;
	}

	if (shaders_window) {
		CreateShaderWindow();
	}	

	return UPDATE_CONTINUE;
}

void ShadersManager::CreateShader(std::string vs_path, std::string fs_path, std::string name)
{
	UUID v_res_uuid = App->resources->Find(vs_path.c_str());
	ResourceShader* v_res = (ResourceShader*)App->resources->Get(v_res_uuid);
	uint vs_ID = v_res->res_shader->shader_id;

	UUID f_res_uuid = App->resources->Find(fs_path.c_str());
	ResourceShader* f_res = (ResourceShader*)App->resources->Get(f_res_uuid);
	uint fs_ID = f_res->res_shader->shader_id;

	ShaderProgram* shader_program = new ShaderProgram();
	shader_program->shader_name = GetFileName(name);
	shader_program->loadProgram(vs_ID, fs_ID);

	App->resources->ImportMaterial(name.c_str(), shader_program);

	shader_vect.push_back(shader_program);


}

bool ShadersManager::CreateShaderFromArray(char* vert_buffer,char* frag_buffer, std::string name)
{

	ShaderProgram* shader_program = new ShaderProgram();
	shader_program->shader_name = name;	
	bool ret = shader_program->loadProgram(vert_buffer, frag_buffer);
	name = "assets/" + name + ".material";
	App->resources->ImportMaterial(name.c_str(),shader_program);

	shader_vect.push_back(shader_program);

	return ret;
}
bool ShadersManager::CreateDefaultShader()
{
	
	default_shader.shader_name = "Default Shader";
	//Success flag
	GLint programSuccess = GL_TRUE;

	//Generate program
	default_shader.mProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 texCoord;\n"
		"layout(location = 3) in vec4 color;\n"
		"out float ourTime;\n"
		"out vec4 ourColor;\n"
		"out vec3 ourNormal;\n"
		"out vec2 TexCoord;\n"
		"uniform float _time;\n"
		"uniform vec4 _color;\n"
		"uniform mat4 model;\n"
		"uniform mat4 viewproj;\n"
		"uniform mat4 view;\n"
		"void main()\n"
		"{\n"
		"gl_Position = viewproj *  model * vec4(position.x,position.y,position.z, 1.0f);\n"
			"ourColor = _color;\n"
			"TexCoord = texCoord;\n"
			"ourTime = _time;\n"
			"ourNormal = mat3(model) * normal;"
		"}\n"
	};

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		ShaderLog(vertexShader);
		return false;
	}

	//Attach vertex shader to program
	glAttachShader(default_shader.mProgramID, vertexShader);

	//Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Get fragment source
	const GLchar* fragmentShaderSource[] =
	{
		"#version 330 core\n"
		"in vec4 ourColor;\n"
		"in float ourTime;\n"
		"in vec2 TexCoord;\n"
		"in vec3 ourNormal;\n"
		"out vec4 color;\n"
		"uniform sampler2D _texture;\n"
		"void main()\n"
		"{\n"
		"vec3 lightDir = vec3(1);\n"
		"float angle = dot(lightDir, ourNormal);\n"
		"color = texture(_texture, TexCoord) * ourColor * vec4(angle,angle,angle,1);\n"
		"}\n"
	};

	//Set fragment source
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	//Compile fragment source
	glCompileShader(fragmentShader);

	//Check fragment shader for errors
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		
		ShaderLog(fragmentShader);
		return false;
	}

	//Attach fragment shader to program
	glAttachShader(default_shader.mProgramID, fragmentShader);

	//Link program
	glLinkProgram(default_shader.mProgramID);

	//Check for errors
	glGetProgramiv(default_shader.mProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		ProgramLog(default_shader.mProgramID);
		return false;
	}
	default_shader.fragmentID = fragmentShader;
	default_shader.vertexID = vertexShader; 
	
	UUID obj_uuid = IID_NULL;
	UUID null_uuid; UuidCreateNil(&null_uuid);
	RPC_STATUS stat;

	//TODO probably dont need
	if (UuidCompare(&obj_uuid, &null_uuid, &stat) == 0) {
		Resource* res = App->resources->CreateNewResource(Resource::shader_program);
		res->file = "default.material";
		res->exported_file = "default.material";
		res->type = Resource::shader_program;
		obj_uuid = res->uuid;

	}

	shader_vect.push_back(&default_shader);
	return true;
	
}

void ShadersManager::ShowVertexShadersFolder(char* file_type, TextEditor* shader_editor)
{

	char* shaders_list = "Vertex_Shaders";
	if (strcmp(file_type, "vrsh") == 0) {
		shaders_list = "Vertex_Shaders";
	}
	else if (strcmp(file_type, "frsh") == 0) {
		shaders_list = "Fragment_Shaders";
	}

	std::vector<std::string> path_vect;
	std::string popupElements;
	std::string path = ASSETS_PATH;
	for (auto & p : fs::directory_iterator(path)) {

		//GET STRING
		const wchar_t* temp = p.path().c_str();
		std::wstring ws(temp);
		std::string str(ws.begin(), ws.end());
		str = NormalizePath(str.c_str());

		std::string file_extension = GetExtension(str);
		if (!strcmp(file_extension.c_str(), file_type)) {

			path_vect.push_back(str);
			popupElements += GetFileName(str);
			popupElements += '\0';
		}
	}

	int shader_pos = 0;
	
	static char* shader_text = GetShaderText(path_vect[shader_pos]);
	
	if (ImGui::Combo(shaders_list, &shader_pos, popupElements.c_str())) {
		shader_text = GetShaderText(path_vect[shader_pos]);
		shader_editor->SetText("");
		shader_editor->InsertText(shader_text);
	}
}

void ShadersManager::CreateShaderWindow()
{
	static bool cant_create_shader = false;

	float2 next_win_size = float2(1200, 650);
	ImGui::SetNextWindowPos(ImVec2((App->window->width / 2) - next_win_size.x / 2, (App->window->height / 2) - next_win_size.y / 2));
	ImGui::SetNextWindowSize(ImVec2(next_win_size.x, next_win_size.y));
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.50f, 0.5f, 0.5f, 1.0f);
	if (ImGui::Begin("Shaders", &shaders_window)) {

		ImGui::Columns(2, "shader_type");
		ImGui::Separator();
		static char temp_name[32] = "NewVertexShader";
		static char temp_name2[32] = "NewFragmentShader";
		char* names[2];
		names[0] = temp_name;
		names[1] = temp_name2;
		char* type = "vrsh";
		char* name = "VertexShader";
		TextEditor* shader_editor = &vertex_editor;
		
		for (int i = 0; i < 2; i++) {
			ImGui::PushID(i);
			ImGui::Text(name);

			if (!load_shader[i] && !new_shader[i]) {
				
				if (ImGui::Button("Load")) {					
					App->shaders->load_shader[i] = true;
				}

				ImGui::SameLine();			
				
				if (ImGui::Button("New")) {
					new_shader[i] = true;				
				}
			}

			else if(load_shader[i] && !new_shader[i]) {
				
				ShowVertexShadersFolder(type, shader_editor);
				ImGui::InputText("Name", names[i], IM_ARRAYSIZE(temp_name));

				if (ImGui::Button("Back")) {
					new_shader[i] = false;
					App->shaders->load_shader[i] = false;
				}

				ImGuiIO& io = ImGui::GetIO();
				ImGui::PushFont(io.Fonts->Fonts[1]);
				shader_editor->Render(name, ImVec2(ImGui::GetColumnWidth(i)-16,440));
				ImGui::PopFont();
			}

			else if (!load_shader[i] && new_shader[i]) {

				ImGui::InputText("Name", names[i], IM_ARRAYSIZE(temp_name));

				if (ImGui::Button("Back")) {
					new_shader[i] = false;
					App->shaders->load_shader[i] = false;
				}

				ImGuiIO& io = ImGui::GetIO();
				ImGui::PushFont(io.Fonts->Fonts[1]);
				shader_editor->Render(name, ImVec2(ImGui::GetColumnWidth(i) - 16, 440));
				ImGui::PopFont();				
			}
						
			ImGui::PopID();

			type = "frsh";
			name = "FragmentShader";
			ImGui::NextColumn();
			shader_editor = &fragment_editor;		
		}

		ImGui::SetCursorPosY(532);
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Columns(2, "", false);

		ImGui::SetCursorPosX(16);
		
		static char buf[32] = "NewShader";
		ImGui::InputText("Material_Name", buf, IM_ARRAYSIZE(buf));
		ImGui::SetCursorPosX(16); 

		if (ImGui::Button("CreateShader", ImVec2(128, 32))) {

			std::string name = &buf[0];

			if (CreateShaderFromArray((char*)vertex_editor.GetText().c_str(), (char*)fragment_editor.GetText().c_str(), name)) {

				Data json_shader;

				std::vector<ShaderProgram*>::iterator item = shader_vect.end();
				item--;
				uint new_fragment_id = (*item)->fragmentID;
				uint new_vertex_id = (*item)->vertexID;

				std::string vertex_path = ASSETS_PATH + std::string(temp_name) + ".vrsh";
				std::string fragment_path = ASSETS_PATH + std::string(temp_name2) + ".frsh";

				json_shader.CreateSection("Material");
				json_shader.AddString("Name", name);
				json_shader.AddString("Vertex_shader", vertex_path);
				json_shader.AddString("Fragment_shader", fragment_path);
				json_shader.CloseSection();

				json_shader.SaveAsJSON(std::string(ASSETS_PATH + name + ".material"));


				uint vertex_buffer_size = strlen(vertex_editor.GetText().c_str());
				uint fragment_buffer_size = strlen(fragment_editor.GetText().c_str());

				char* vertex_buffer = new char[vertex_buffer_size];
				char* fragment_buffer = new char[fragment_buffer_size];

				memcpy(vertex_buffer, vertex_editor.GetText().c_str(), vertex_buffer_size);
				memcpy(fragment_buffer, fragment_editor.GetText().c_str(), fragment_buffer_size);

				//VERTEX FILE
				FILE* pFile = fopen(vertex_path.c_str(), "wb");
				fwrite(vertex_buffer, sizeof(char), vertex_buffer_size, pFile);
				fclose(pFile);

				//SHADER FILE
				pFile = fopen(fragment_path.c_str(), "wb");
				fwrite(fragment_buffer, sizeof(char), fragment_buffer_size, pFile);
				fclose(pFile);

				UUID vert_uuid = App->resources->Find(vertex_path.c_str());
				UUID null_uuid; UuidCreateNil(&null_uuid);
				RPC_STATUS stat;

				//TODO probably dont need
				Shader* new_vertex = new Shader();
				if (UuidCompare(&vert_uuid, &null_uuid, &stat) == 0) {
					Resource* res = App->resources->CreateNewResource(Resource::vertex_shader);
					res->file = vertex_path;
					res->exported_file = vertex_path;
					res->type = Resource::vertex_shader;					
					new_vertex->shader_id = new_vertex_id;
					((ResourceShader*)res)->res_shader = new_vertex;
				}
				else {
					new_vertex->shader_id = new_vertex_id;
					((ResourceShader*)App->resources->Get(vert_uuid))->res_shader = new_vertex;
				}

				App->filesystem->CreateMeta(vertex_path);

				UUID obj_uuid = App->resources->Find(fragment_path.c_str());
			

				//TODO probably dont need
				Shader* new_fragment = new Shader();
				if (UuidCompare(&obj_uuid, &null_uuid, &stat) == 0) {
					Resource* res = App->resources->CreateNewResource(Resource::fragment_shader);
					res->file = fragment_path;
					res->exported_file = fragment_path;
					res->type = Resource::fragment_shader;
					
					new_fragment->shader_id = new_fragment_id;
					((ResourceShader*)res)->res_shader = new_fragment;
				}
				else {
					new_fragment->shader_id = new_fragment_id;
					((ResourceShader*)App->resources->Get(vert_uuid))->res_shader = new_fragment;
				}
				
				App->filesystem->CreateMeta(fragment_path);
			}

			else {
				cant_create_shader = true;
			}
		}

		ImGui::Columns(1);
		
		if (cant_create_shader) {

			float2 next_win_size = float2(500, 200);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.68f, 0.69f, 0.69f, 0.91f);
			ImGui::SetNextWindowPos(ImVec2((App->window->width / 2) - next_win_size.x / 2, (App->window->height / 2) - next_win_size.y / 2));
			ImGui::SetNextWindowSize(ImVec2(next_win_size.x, next_win_size.y));

			if (ImGui::Begin("ERROR!", &cant_create_shader)) {

				ImGui::Text("Can't compile or link the shaders");
				ImGui::End();
			}
		}
	}

	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	ImGui::End();	
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.68f, 0.69f, 0.69f, 0.91f);

}

void ShadersManager::UpdateShaderWindow(ComponentMaterial* comp,ShaderType type)
{
	static uint i;
	static uint n;
	static char* path;
	static char* buffer;
	static TextEditor shader_editor;
	static bool is_default_shader = false;

	if (type == ShaderType::vertex_shader) {
		i = comp->shader->vertexID;
		n = comp->shader->fragmentID;		
	}

	if (type == ShaderType::pixel_shader) {
		i = comp->shader->fragmentID;
		n = comp->shader->vertexID;
	}

	float2 next_win_size = float2(600, 616);
	ImGui::SetNextWindowPos(ImVec2((App->window->width / 2) - next_win_size.x / 2, (App->window->height / 2) - next_win_size.y / 2), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(next_win_size.x, next_win_size.y), ImGuiCond_Appearing);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.50f, 0.5f, 0.5f, 1.0f);

	ImGuiWindowFlags window_flags = 0;
	if(ImGui::Begin("Change Shader", &shader_change, window_flags) ){

		if (ImGui::Button("Save") && ! is_default_shader) {

			uint buffer_size = strlen(shader_editor.GetText().c_str());

			FILE* pFile = fopen(path, "wb");
			fwrite(shader_editor.GetText().c_str(), sizeof(char), buffer_size, pFile);
			fclose(pFile);

			uint new_shader_id = App->filesystem->shader_importer->loadShaderFromFile(path, type);

			for (std::vector<ShaderProgram*>::iterator item = shader_vect.begin(); item != shader_vect.end(); item++) {

				switch (type) {

				case ShaderType::vertex_shader:
					if ((*item)->vertexID == i) {
						(*item)->UpdateShaderProgram(new_shader_id,0);
					}break;


				case ShaderType::pixel_shader:
					if ((*item)->fragmentID == i) {						
						(*item)->UpdateShaderProgram(0,new_shader_id);						
					}break;

				}
			}

			glDeleteShader(i);

			UUID shader_uuid = App->resources->Find(path);
			((ResourceShader*)App->resources->Get(shader_uuid))->res_shader->shader_id = new_shader_id;
		}
		
		if (set_editor_text) {
		
			path = GetShaderPath(i);

			if (path != nullptr) {

				buffer = GetShaderText(path);
				set_editor_text = false;

				shader_editor.SetText(buffer);
				is_default_shader = false;
			}

			else {

				is_default_shader = true;
				shader_editor.SetText("YOU CAN'T MODIFY THE DEFAULT SHADER");
			}
		}
		
		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[1]);
		shader_editor.Render("Shader Editor");
		ImGui::PopFont();
	}

	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	ImGui::End();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.68f, 0.69f, 0.69f, 0.91f);
}

char * ShadersManager::GetShaderPath(uint ID)
{
	for (std::map<UUID, Resource*>::const_iterator it = App->resources->resources.begin(); it != App->resources->resources.end(); ++it)	{

		if (it->first != IID_NULL && it->second != nullptr &&(it->second->type == Resource::fragment_shader || it->second->type == Resource::vertex_shader)) {

			if (((ResourceShader*)it->second)->res_shader->shader_id == ID)
				return (char*)it->second->file.c_str();
		}
	}

	return nullptr;
}

char * ShadersManager::GetShaderText(std::string path)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;
	std::string string_buffer;
	pFile = fopen(path.c_str(), "rb");

	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = new char[lSize + 1];
	memset(buffer, 0, lSize + 1);

	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);

	fclose(pFile);

	return buffer;
}

void ShadersManager::LoadMaterials(){

	std::vector<std::string> path_vect;
	std::string popupElements;
	std::string path = ASSETS_PATH;

	for (auto & p : fs::directory_iterator(path)) {

		const wchar_t* temp = p.path().c_str();
		std::wstring ws(temp);
		std::string str(ws.begin(), ws.end());
		str = NormalizePath(str.c_str());
		std::string file_extension = GetExtension(str);

		if (!strcmp(file_extension.c_str(), "material")) {

			Data material_data;
			
			//LOAD JSON FILE
			if (material_data.LoadJSON(str)) {

				material_data.EnterSection("Material");
				std::string vertex_shader = material_data.GetString("Vertex_shader");
				std::string fragment_shader = material_data.GetString("Fragment_shader");
				std::string name = material_data.GetString("Name");

				CreateShader(vertex_shader.c_str(), fragment_shader.c_str(), str.c_str());
			}
		}
	}
}

