#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "SDL\include\SDL_opengl.h"



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D( bool start_enabled) : Module( start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	

	//MATHGEO TEST
	vec temp_vec = { 1,0,0 };
	test_sphere1 = new Sphere(temp_vec, 1);
	temp_vec = { -1,0,0 };
	test_sphere2 = new Sphere(temp_vec, 1);

	
	

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		LOG("OpenGL context created");
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;

		
		lights[0].ambient.Set(0.f, 0.f, 0.f, 1.0f);
		lights[0].diffuse.Set(1.f, 1.f, 1.f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 0.0f);
		lights[0].Init();
		
	/*	GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		*/
		lights[0].Active(true);
		
	}
	if (ret) {
		LOG("OpenGL Initizlized correctly");
	}
	// Projection matrix for
	OnResize(App->window->width,App->window->height);

	if (glewInit() != GLEW_OK) {
		LOG("Failed to initialize GLEW\n");
	}
	else {
		LOG("Glew initialized correctly");
	}

	ilInit();
	ilClearColour(255, 255, 255, 000);

	//Check for error
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		
		return false;
	}

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	//lights[0].SetPos(0, 0, 0);
	float x = lights[0].position.x;
	float z = lights[0].position.z;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		z -= 0.1;
		lights[0].SetPos(x, 0, z);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		z += 0.1;
		lights[0].SetPos(x, 0, z);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		x -= 0.1;
		lights[0].SetPos(x, 0, z);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		x += 0.1;
		lights[0].SetPos(x, 0, z);
	}
	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ModuleRenderer3D::Options()
{

	if (ImGui::CollapsingHeader("Renderer"))
	{
		int major=0, minor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MAJOR_VERSION, &minor);
		ImGui::Text("OpenGL Version:");
		ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i.%i", major, minor);
		ImGui::Text("GPU Vendor:");
		ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, (const char*)glGetString(GL_VENDOR));
		ImGui::Text("Model:");
		ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, (const char*)glGetString(GL_RENDERER));
		ImGui::Text("Total VRAM:");
		GLint total_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
			&total_mem_kb);
		ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i MB",total_mem_kb/1024);

		ImGui::Text("Free VRAM:");
		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
			&cur_avail_mem_kb);
		ImGui::SameLine(); ImGui::TextColored({ 1,1,0,1 }, "%i MB", cur_avail_mem_kb / 1024);
		//Checkboxs
	
		static bool depth = glIsEnabled(GL_DEPTH_TEST);
		static bool cull =  glIsEnabled(GL_CULL_FACE);
		static bool light = glIsEnabled(GL_LIGHTING);
		static bool color_material = glIsEnabled(GL_COLOR_MATERIAL);		
		static bool texture = glIsEnabled(GL_TEXTURE);
		if (ImGui::Checkbox("Depth Test", &depth)) {
			if (depth)glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Cull Face", &cull))
		{
			if (cull)glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
		};
		ImGui::SameLine();
		if(ImGui::Checkbox("Lighting", &light))
		{
			if (light)glEnable(GL_LIGHTING);
			else glDisable(GL_LIGHTING);
		};
		
		if (ImGui::Checkbox("Color Material", &color_material))
		{
			if (color_material)glEnable(GL_COLOR_MATERIAL);
			else glDisable(GL_COLOR_MATERIAL);
		};
		ImGui::SameLine();
		if (ImGui::Checkbox("Texture", &texture))
		{
			if (texture)glEnable(GL_TEXTURE);
			else glDisable(GL_TEXTURE);
		};

		if (ImGui::Checkbox("Shaded", &render_fill));
		
			ImGui::SameLine();
		if (ImGui::Checkbox("Wireframe", &render_wireframe));
		
		

	}

	return false;
}

void ModuleRenderer3D::Render(Mesh m)
{
	
	if (m.id_vertexs != NULL) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (m.id_norms != NULL) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m.id_norms);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}

		if (m.id_colors != NULL) {
			glEnableClientState(GL_COLOR_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m.id_norms);
			glColorPointer(3,GL_FLOAT, 0, NULL);
		}

		if (m.id_textures != NULL) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, App->renderer3D->mTextureID);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER,m.id_textures);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		if(m.id_indices != NULL)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
		glDrawElements(GL_TRIANGLES, m.num_indices, GL_UNSIGNED_INT, NULL);

	
		}
	glDisable(GL_TEXTURE_2D);
}

bool ModuleRenderer3D::loadTextureFromFile(char* path)
{
	//Texture loading success
	bool textureLoaded = false;

	//Generate and set current image ID
	uint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	//Load image
	ILboolean success = ilLoadImage(path);

	//Image loaded successfully
	if (success == IL_TRUE)
	{
		//Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE)
		{
			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
			//Create texture from file pixels
			textureLoaded = true;
		}

		//Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	//Report error
	if (!textureLoaded)
	{
		
	}

	return textureLoaded;
}

bool ModuleRenderer3D::loadTextureFromPixels32(GLuint * pixels, GLuint width, GLuint height)
{

	//Free texture if it exists
	//freeTexture();

	//Get texture dimensions
	/*glGenTextures(1, &mTextureID);

	//Bind texture ID
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture( GL_TEXTURE_2D, NULL );
	*/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, NULL);
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }

    return true;
}
