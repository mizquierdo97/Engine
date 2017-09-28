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
	sphere = new Sphere({ 0,0,-10 }, 3);
	sphere->Triangulate(&vect, &norms, NULL, 6144, false);

	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vect.size()*sizeof(float)*3,&vect[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, norms.size()*sizeof(float)*3, &norms[0], GL_STATIC_DRAW);
	
	
	indices = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
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
	};

	
	
	glGenBuffers(1, (GLuint*) &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *cube_vertices.size() * 3, &cube_vertices[0], GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*) &my_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(),&indices[0], GL_STATIC_DRAW);

	


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
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	
	// Draw the triangle !

	glBegin(GL_TRIANGLES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);

	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);



	glEnd();
	
	App->renderer3D->Render();
	


	return UPDATE_CONTINUE;
}

bool ModuleWorld::CleanUp()
{
	return true;
}
