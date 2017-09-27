#include "Application.h"
#include "ModuleWorld.h"

ModuleWorld::ModuleWorld(bool start_enable)
{
	
}

ModuleWorld::~ModuleWorld()
{
}

bool ModuleWorld::Init()
{
	
	return true;
}

bool ModuleWorld::Start() {
	sphere = new Sphere({ 0,1,-3 }, 1);
	sphere->Triangulate(vect, norms, NULL, 6144, false);

	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vect), vect, GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(norms), norms, GL_STATIC_DRAW);

	return true;
}

update_status ModuleWorld::PreUpdate(float dt)
{

	
	
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorld::PostUpdate(float dt)
{
	
App->renderer3D->Render();
	


	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}
