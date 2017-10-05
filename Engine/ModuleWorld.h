#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "Texture.h"

class ModuleWorld : public Module
{
public: 

	ModuleWorld(bool start_enabled = true);
	~ModuleWorld();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ImportMesh(char* path);
	bool Options();
public:

	Sphere* sphere;
	vec* temp;

	GLuint vertexbuffer;
	GLuint normalbuffer;
	std::vector<vec> vect;
	std::vector<vec> norms;
	std::vector<unsigned int> indices;
	GLuint my_indices = 0;
	std::vector<GLfloat> cube_vertices;


	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	uint ImageName;
	GLuint coords_id;
	std::vector<GLfloat> tex_coords;

	Texture* world_texture;
};
