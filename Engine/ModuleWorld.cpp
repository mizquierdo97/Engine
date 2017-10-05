#include "Application.h"
#include "ModuleWorld.h"
#include "Primitives.h"
#include "imgui_dock.h"

ModuleWorld::ModuleWorld(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleWorld::~ModuleWorld()
{
}

bool ModuleWorld::Init()
{
	
	return true;
}

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128
bool ModuleWorld::Start() {
	
	App->renderer3D->loadTextureFromFile("Baker_house2.png");

/*	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

*/



	indices = {
		// front
		3, 0, 1,
		3, 1, 2,
		// top
		7, 3, 2,
		7, 2, 6,
		// back
		4, 7, 6,
		4, 6, 5,
		// bottom
		0,4, 5,
		0, 5, 1,
		// left
		12, 15, 14,
		12, 14, 13,
		// right
		11, 8, 9,
		11, 9, 10,

		
	};

	cube_vertices = {
		// front
		2.0, -1.0,  1.0,
		4.0, -1.0,  1.0,
		4.0,  1.0,  1.0,
		2.0,  1.0,  1.0,
		// back
		2.0, -1.0, -1.0,
		4.0, -1.0, -1.0,
		4.0,  1.0, -1.0,
		2.0,  1.0, -1.0,

		4.0, -1.0, 1.0,
		4.0, -1.0, -1.0,
		4.0,  1.0, -1.0,
		4.0,  1.0, 1.0,

		2.0, 1.0, -1.0,
		2.0, 1.0, 1.0,
		2.0,  -1.0, 1.0,
		2.0,  -1.0, -1.0, //15
	};

	tex_coords = {
	
		1.f,  1.f,
		0.f,  1.f,  
		0.f,  0.f,  
		1.f,  0.f,  

		1.f,  0.f,
		0.f,  0.f,  
		0.f,  1.f,  
		1.f,  1.f, 

		// extra coords for left - right
		1.f,  1.f, 
		0.f,  1.f, 
		0.f,  0.f,  
		1.f,  0.f,  

		0.f,  1.f,  
		1.f,  1.f,  
		1.f,  0.f, 
		0.f,  0.f, 
	
		1.f,  1.f,
		0.f,  1.f,
		0.f,  0.f,
		1.f,  0.f,

		0.f,  1.f,
		1.f,  1.f,
		1.f,  0.f,
		0.f,  0.f,
		

	};

	glGenBuffers(1, (GLuint*)&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *cube_vertices.size() * 3, &cube_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&coords_id);
	glBindBuffer(GL_ARRAY_BUFFER, coords_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *tex_coords.size()*2, &tex_coords[0], GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*)&my_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	world_texture = new Texture();
	world_texture->Create(nullptr, App->window->width, App->window->height);

	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{
	if (App->input->file_dropped) {
		ImportMesh(App->input->dropped_filedir);
		App->input->file_dropped = false;
	}
	
	world_texture->Bind();
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float dt)
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, ImageName);

	glBegin(GL_TRIANGLES);
	 glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);  glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);


	glVertex3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f); glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f,  0.0f); glTexCoord2f(1.0f, 1.0f); //1
	glVertex3f(1.0f, 1.0f, 0.0f); 


	glVertex3f(0.0f, 1.0f, -1.0f); glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);//2
	glVertex3f(1.0f, 1.0f, -1.0f); glTexCoord2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 1.0f);//1
	glVertex3f(1.0f, 0.0f, -1.0f); 


	glVertex3f(0.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 0.0f); //2
	glVertex3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f); glTexCoord2f(1.0f, 1.0f); //1
	glVertex3f(1.0f, 1.0f, -1.0f); 


	glVertex3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f); glTexCoord2f(1.0f, 0.0f);//2
	glVertex3f(1.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f);//1
	glVertex3f(01.0f, 0.0f, -1.0f);


	glVertex3f(1.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f); glTexCoord2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f); glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); 

	glVertex3f(0.0f, 1.0f, -1.0f); glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	

	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D,App->renderer3D->tex->GetTexture());

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,coords_id);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	//RENDER MESHES
	std::vector<Mesh>::iterator temp = App->assimp->mesh_vector->begin();
	while (temp != App->assimp->mesh_vector->end()) {		
		App->renderer3D->Render(*temp);
		temp++;
	}
	
	world_texture->Unbind();

	ImGui::Render();
	

	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}

void ModuleWorld::ImportMesh(char * path)
{
	App->assimp->ImportMesh(path);
}


bool ModuleWorld::Options()
{
	if (ImGui::BeginDock("Scene", false, false/*, App->IsPlaying()*/, ImGuiWindowFlags_HorizontalScrollbar)) {
		ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)world_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::EndDock();
	return true;
}