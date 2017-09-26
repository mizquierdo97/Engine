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
	sphere = new Sphere({ 0,0,0 }, 1);
	
	sphere->Triangulate(vect,norms, NULL, 6144,false);
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

	
	


	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vect), vect, GL_STATIC_DRAW);
	

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(norms), norms, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);



	glEnableVertexAttribArray(2);

	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_TRUE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6144);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6144); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);

	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}
