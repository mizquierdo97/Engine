#include "Application.h"
#include "Object.h"

Mesh CreateCube()
{

	Mesh m;

	std::vector<GLuint>indices = {
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

	std::vector<float>cube_vertices = {
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

	std::vector<float> tex_coords = {

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

	};


	m.num_vertexs = cube_vertices.size() / 3;
	m.vertexs = new float[m.num_vertexs * 3];
	memcpy(m.vertexs, &cube_vertices[0], sizeof(float) * m.num_vertexs * 3);

	m.texture_coords = new float[m.num_vertexs * 2];
	memcpy(m.texture_coords, &tex_coords[0], sizeof(float) * m.num_vertexs * 2);

	m.num_indices = indices.size();
	m.indices = new uint[m.num_indices]; // assume each face is a triangle
	memcpy(m.indices, &indices[0], sizeof(uint) * m.num_indices);

	glGenBuffers(1, (GLuint*)&m.id_vertexs);
	glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&m.id_textures);
	glBindBuffer(GL_ARRAY_BUFFER, m.id_textures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *m.num_vertexs * 2, m.texture_coords, GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*)&m.id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m.num_indices, m.indices, GL_STATIC_DRAW);


	return m;
}

Mesh CreateCylinder()
{
	float rad = 1;
	float height = 2;
	int num = 20;

	std::vector<float> cyl_vertices;

	std::vector<uint> indices;
	std::vector<float> texture_coords;
	cyl_vertices.push_back(0.0f);
	cyl_vertices.push_back(0.0f);
	cyl_vertices.push_back(0.0f);
	float angle = DegToRad(360.0f / (float)num);
	for (int i = 0; i < num; i++) {

		float x_pos = cos(angle*i) *rad;
		float y_pos = sin(angle*i) *rad;

		cyl_vertices.push_back(x_pos);
		cyl_vertices.push_back(y_pos);
		cyl_vertices.push_back(0.0f);

	}
	cyl_vertices.push_back(0.0f);
	cyl_vertices.push_back(0.0f);
	cyl_vertices.push_back(-height);
	for (int i = 0; i < num; i++) {

		float x_pos = cos(angle*i) *rad;
		float y_pos = sin(angle*i) *rad;

		cyl_vertices.push_back(x_pos);
		cyl_vertices.push_back(y_pos);
		cyl_vertices.push_back(-height);

	}
	for (int i = 0; i < num; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		if (i + 2 == num + 1)
			indices.push_back(1);
		else
			indices.push_back(i + 2);
	}
	for (int i = num + 1; i < num * 2 + 1; i++) {
		indices.push_back(i + 1);
		indices.push_back(num + 1);
		if (i + 2 == num * 2 + 2)
			indices.push_back(num + 2);
		else
			indices.push_back(i + 2);
	}

	for (int i = 1; i <= num; i++) {

		indices.push_back(i);
		indices.push_back(i + num + 1);
		if (i + 1 == num + 1)
			indices.push_back(1);
		else
			indices.push_back(i + 1);

		indices.push_back(i + num + 1);
		if (i + 1 == num + 1) {
			indices.push_back(num + 2);
			indices.push_back(1);
		}
		else {
			indices.push_back(i + num + 2);
			indices.push_back(i + 1);
		}
	}

	Mesh m;

	m.num_vertexs = cyl_vertices.size() / 3;
	m.vertexs = new float[m.num_vertexs * 3];
	memcpy(m.vertexs, &cyl_vertices[0], sizeof(float) * m.num_vertexs * 3);


	m.num_indices = indices.size();
	m.indices = new uint[m.num_indices]; // assume each face is a triangle
	memcpy(m.indices, &indices[0], sizeof(uint) * m.num_indices);

	glGenBuffers(1, (GLuint*)&m.id_vertexs);
	glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*)&m.id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m.num_indices, m.indices, GL_STATIC_DRAW);

	return m;
}

int CreateObject()
{
	GameObject* new_object = new GameObject();
	UuidCreate(&new_object->obj_uuid);
	App->world->obj_vector.push_back(new_object);
	
	return 0;
}
void CreateObject(GameObject* obj)
{
	GameObject* new_object = new GameObject();
	new_object = obj;
	UuidCreate(&new_object->obj_uuid);
	if (new_object->obj_parent!= nullptr) {
		new_object->obj_parent->obj_childs.push_back(new_object);
	}
	else {
		App->world->obj_vector.push_back(new_object);
	}
	App->world->quadtree.Insert(new_object);

	
}


 Mesh CreateAABB(AABB b_box) {
	vec vertexs[36];

	b_box.Triangulate(1, 1, 1, &vertexs[0], nullptr, nullptr, false);
	Mesh m;

	m.num_vertexs = 36;
	m.vertexs = new float[m.num_vertexs * 3];

	for (int i = 0; i < 36; i++) {
		m.vertexs[i * 3] = vertexs[i].x;
		m.vertexs[i * 3 + 1] = vertexs[i].y;
		m.vertexs[i * 3 + 2] = vertexs[i].z;

	}
	glGenBuffers(1, (GLuint*)&m.id_vertexs);
	glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);

	return m;

};

 Mesh UpdateAABB(Mesh m, AABB b_box) {
	 vec vertexs[36];

	 b_box.Triangulate(1, 1, 1, &vertexs[0], nullptr, nullptr, false);

	 for (int i = 0; i < 36; i++) {
		 m.vertexs[i * 3] = vertexs[i].x;
		 m.vertexs[i * 3 + 1] = vertexs[i].y;
		 m.vertexs[i * 3 + 2] = vertexs[i].z;

	 }
	
	glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);
	

	 return m;

 };