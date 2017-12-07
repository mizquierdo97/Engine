#include "Application.h"
#include "ResourceShader.h"
#include "ShadersManager.h"

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
	CreateShader("Assets/VertexShader.vrsh", "Assets/Shader.frsh", "DefaultTest");
	CreateShader("Assets/VertexShader.vrsh", "Assets/Red.frsh", "Red");
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
	time_dt += dt;
	while (item != shader_vect.end()) {
		(*item)->bind();

		//TIME
		
		GLint timeLoc = glGetUniformLocation((*item)->mProgramID, "_time");
		glUniform1f(timeLoc, sin(time_dt));



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
	shader_program->shader_name = name;
	shader_program->loadProgram(vs_ID, fs_ID);
	shader_vect.push_back(shader_program);


}

void ShadersManager::CreateShaderFromArray(char* vert_buffer,char* frag_buffer, std::string name)
{

	ShaderProgram* shader_program = new ShaderProgram();
	shader_program->shader_name = name;
	shader_program->loadProgram(vert_buffer, frag_buffer);
	shader_vect.push_back(shader_program);


}
bool ShadersManager::CreateDefaultShader()
{
	
	default_shader.shader_name = name;
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
		"out vec2 TexCoord;\n"
		"uniform float _time;\n"
		"uniform vec4 _color;\n"
		"uniform mat4 model;\n"
		"uniform mat4 viewproj;\n"
		"void main()\n"
		"{\n"
		"gl_Position = viewproj *  model * vec4(position.x,position.y,position.z, 1.0f);\n"
			"ourColor = _color;\n"
			"TexCoord = texCoord;\n"
			"ourTime = _time;\n"
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
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		//printShaderLog(vertexShader);
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
		"in float ourTime;"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D _texture;\n"
		"void main()\n"
		"{\n"
		"color = texture(_texture, TexCoord) * vec4(ourColor.r,ourColor.g,ourColor.b,ourColor.a);\n"
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
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		//printShaderLog(fragmentShader);
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
		printf("Error linking program %d!\n", default_shader.mProgramID);
		//printProgramLog(mProgramID);
		return false;
	}
	default_shader.fragmentID = fragmentShader;
	default_shader.vertexID = vertexShader; 
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
	float2 next_win_size = float2(1200, 616);
	ImGui::SetNextWindowPos(ImVec2((App->window->width / 2) - next_win_size.x / 2, (App->window->height / 2) - next_win_size.y / 2));
	ImGui::SetNextWindowSize(ImVec2(next_win_size.x, next_win_size.y));
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.50f, 0.5f, 0.5f, 1.0f);
	if (ImGui::Begin("Shaders", &shaders_window)) {

		ImGui::Columns(2, "shader_type");
		ImGui::Separator();

		char* type = "vrsh";
		char* name = "VertexShader";
		TextEditor* shader_editor = &vertex_editor;
		
		for (int i = 0; i < 2; i++) {

			ImGui::Text(name);
			if (!App->shaders->vertex_shader_window[i]) {

				ImGui::PushID(i);
				if (ImGui::Button("Load")) {
					
					App->shaders->vertex_shader_window[i] = true;
				}

				ImGui::SameLine();
				ImGui::Button("New");
				ImGui::PopID();
			}
			else {

				ShowVertexShadersFolder(type, shader_editor);
				ImGuiIO& io = ImGui::GetIO();
				ImGui::PushFont(io.Fonts->Fonts[1]);
				
				shader_editor->Render(name, ImVec2(ImGui::GetColumnWidth(i)-16,440));
				ImGui::PopFont();
			}
					
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
		
		static char buf[32] = "dummy";
		ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf));
		ImGui::SetCursorPosX(16);
		if (ImGui::Button("CreateShader", ImVec2(128, 32))) {
			std::string name = &buf[0];
			CreateShaderFromArray((char*)vertex_editor.GetText().c_str(), (char*)fragment_editor.GetText().c_str(),name);
		}
		ImGui::Columns(1);
		
	}
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	ImGui::End();
	
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.68f, 0.69f, 0.69f, 0.91f);

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
	buffer = new char[lSize + 1];// (char*)malloc(sizeof(char)*lSize);
	memset(buffer, 0, lSize + 1);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);

	fclose(pFile);

	return buffer;
}

