#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "imgui_dock.h"
#include "SDL\include\SDL_opengl.h"
#include "Texture.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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
		

		texture = true;
		render_fill = true;
		render_wireframe = false;


		GLfloat LightModelAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;

		
		lights[0].ambient.Set(0.f, 0.f, 0.f, 1.0f);
		lights[0].diffuse.Set(1.f, 1.f, 1.f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 0.0f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
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
	iluInit();
	ilutInit();
	ilClearColour(255, 255, 255, 000);

	//Check for error
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		
		return false;
	}

	//temp_shader.loadProgram();

	return ret;
}

bool ModuleRenderer3D::Start()
{
	GLubyte defaultImage[128][128][4];
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {
			
			defaultImage[i][j][0] = (GLubyte)255;
			defaultImage[i][j][1] = (GLubyte)255;
			defaultImage[i][j][2] = (GLubyte)255;
			defaultImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &DefaultTexture);
	glBindTexture(GL_TEXTURE_2D, DefaultTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128,
		0, GL_RGBA, GL_UNSIGNED_BYTE, defaultImage);

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	App->camera->UpdateCamProp();
	glLoadMatrixf(App->camera->dummyfrustum->cam_frustum.ViewProjMatrix().Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->dummyfrustum->cam_frustum.ViewProjMatrix().Transposed().ptr());
	*/
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
	
	ProjectionMatrix = App->camera->dummyfrustum->cam_frustum.ViewProjMatrix().Transposed();//perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.ptr());

	App->camera->UpdateCamProp();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ModuleRenderer3D::Options()
{

	if (ImGui::BeginDock("Renderer", false, false, false,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_ShowBorders)) {

	
		static bool depth = glIsEnabled(GL_DEPTH_TEST);
		static bool cull =  glIsEnabled(GL_CULL_FACE);
		static bool light = glIsEnabled(GL_LIGHTING);
		static bool color_material = glIsEnabled(GL_COLOR_MATERIAL);		
		
		if (ImGui::Checkbox("Depth Test", &depth)) {
			if (depth)glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
		}

		if(ImGui::Checkbox("Cull Face", &cull))
		{
			if (cull)glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
		};
		
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
	
		if (ImGui::Checkbox("Texture", &texture));
		if (ImGui::Checkbox("Shaded", &render_fill));		
		if (ImGui::Checkbox("Wireframe", &render_wireframe));
		if (ImGui::Checkbox("Normals", &App->gui->show_normals));		
		if (ImGui::Checkbox("Debug Draw", &debug_draw));
		
		ImGui::EndDock();

	}

	return false;
}

void ModuleRenderer3D::Render(ComponentMesh* comp)
{	

	ShaderProgram* shader;
	shader = &App->shaders->default_shader;
	

	
	float4x4 matrixfloat = comp->GetParent()->GetTransform()->GetMatrix();
	GLfloat matrix[16] =
	{
		matrixfloat[0][0],matrixfloat[1][0],matrixfloat[2][0],matrixfloat[3][0],
		matrixfloat[0][1],matrixfloat[1][1],matrixfloat[2][1],matrixfloat[3][1],
		matrixfloat[0][2],matrixfloat[1][2],matrixfloat[2][2],matrixfloat[3][2],
		matrixfloat[0][3],matrixfloat[1][3],matrixfloat[2][3],matrixfloat[3][3]
	};
	

	Mesh m = ((ResourceMesh*)comp->GetResource())->obj_mesh;

	if (m.texture_coords != nullptr) {
		if (texture && comp->GetParent()->GetMaterial() != nullptr) {
			ComponentMaterial* mat = comp->GetParent()->GetMaterial();
			if (mat->shader.mProgramID != 0) {
				shader = &mat->shader;
			}
		}
	}
	shader->bind();
	uint offset = sizeof(float)* (3 + 3 + 2 + 4);
	if (m.texture_coords == nullptr)
		offset -= sizeof(float) * 2;
	if (m.norms == nullptr)
		offset -= sizeof(float) * 3;


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, m.id_buffer);
	glEnableVertexAttribArray(0);    //We like submitting vertices on stream 0 for no special reason
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, offset, 0);   //The starting point of the VBO, for the vertices
	
	if (m.norms != nullptr) {
		glEnableVertexAttribArray(1);    //We like submitting normals on stream 1 for no special reason
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) * 3));     //The starting point of normals, 12 bytes away
	}
	if (m.texture_coords != nullptr) {
		glEnableVertexAttribArray(2);    //We like submitting texcoords on stream 2 for no special reason
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) * (3 + 3)));   //The starting point of texcoords, 24 bytes away
	}
	glEnableVertexAttribArray(3);    //We like submitting texcoords on stream 2 for no special reason
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) *(3 + 3 + 2)));
	
	
	if (m.id_indices != NULL) {
	
		
		if (m.texture_coords != nullptr) {
			if (texture && comp->GetParent()->GetMaterial() != nullptr) {
				ComponentMaterial* mat = comp->GetParent()->GetMaterial();
				ResourceTexture* tex_res = ((ResourceTexture*)comp->GetParent()->GetMaterial()->GetResource());
				if (tex_res != nullptr) {
					Texture* temp_tex = tex_res->res_tex;
					UseTexture(shader->mProgramID, temp_tex->GetTexture());
				}
			}
			else {
				UseTexture(shader->mProgramID);
				
			}
		}
		else {
			UseTexture(shader->mProgramID);
		}
		
		float modelview[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

		float projectview[16];
		glGetFloatv(GL_PROJECTION_MATRIX, projectview);




		GLint modelLoc = glGetUniformLocation(shader->mProgramID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrix);

		GLint viewLoc = glGetUniformLocation(shader->mProgramID, "viewproj");

		glUniformMatrix4fv(viewLoc, 1, GL_TRUE,App->camera->dummyfrustum->cam_frustum.ViewProjMatrix().ptr());


		
	

		if (m.id_indices != NULL)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
		glDrawElements(GL_TRIANGLES, m.num_indices, GL_UNSIGNED_INT, NULL);
		glPopMatrix();

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	if (m.norms != nullptr && App->gui->show_normals)
		App->physics->DrawNormals(comp);


	shader->unbind();
	Color color = Color(0, 0, 1);	
	if (App->renderer3D->debug_draw) {
		
		if (comp->GetParent() == App->world->GetSelectedObject())
			color = Color(1, 0, 0);
		App->renderer3D->DebugDraw(comp->GetParent()->GetGlobalBBox(), color);
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ModuleRenderer3D::RenderMesh(Mesh * m)
{
	if (m->id_indices != NULL) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m->id_vertexs);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (m->id_norms != NULL) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_norms);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}

		if ( m->id_textures != NULL) {
			//Texture* temp_tex = comp->parent->GetMaterial()->obj_tex;

			if (texture) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
				//glBindTexture(GL_TEXTURE_2D, temp_tex->GetTexture());
			}
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_textures);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		if (m->id_indices != NULL)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
		glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL);


	}
	else {
		if (0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, m->id_vertexs);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, m->num_vertexs);

	}
	glDisable(GL_TEXTURE_2D);


}



void ModuleRenderer3D::UseTexture(uint shader_id,uint i)
{
	if (i == 0)
		i = DefaultTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, i);
	//change shader
	uint tex_id = glGetUniformLocation(shader_id, "_texture");
	glUniform1i(tex_id, 0);
}

ComponentCamera * ModuleRenderer3D::GetActiveCamera()
{
	ComponentCamera* ret = nullptr;
	std::vector<GameObject*>::iterator item = App->world->obj_vector.begin();
	while (item != App->world->obj_vector.end()) {
		ret = (*item)->GetCamera();
		if (ret != nullptr)return ret;
		item++;
	}
	return ret;
}


void ModuleRenderer3D::DebugDraw(const AABB & aabb, Color color, const float4x4& transform)
{
	static float3 corners[8];
	aabb.GetCornerPoints(corners);

	
	DebugDrawBox(corners, color);
	//glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


void ModuleRenderer3D::DebugDrawBox(const float3* corners, Color color)
{
	glColor4f(color.r, color.g, color.b,1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);
	
	
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
	
	glEnd();
	//glEnable(GL_CULL_FACE);
}
