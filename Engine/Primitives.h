#pragma once
#include "ModuleAssimp.h"

class Primitives {
public:
	void CreateCube() {

		Mesh m;
		std::vector<uint> indices = {
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
		std::vector<float> cube_vertices = {
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
		m.num_vertexs = 8;
		m.num_indices = 36;
		
		m.vertexs = new float[m.num_vertexs*3];
		m.indices = new uint[m.num_indices];
		memcpy(m.vertexs,&cube_vertices[0], sizeof(float) * m.num_vertexs * 3);
		for (uint i = 0; i < 36; ++i)
		{			
				memcpy(&m.indices[i],&indices[i], sizeof(uint));
		}
		glGenBuffers(1, (GLuint*)&(m.id_vertexs));
		glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);

		// Buffer for indices
		glGenBuffers(1, (GLuint*)&m.id_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *m.num_indices, m.indices, GL_STATIC_DRAW);

		App->assimp->mesh_vector->push_back(m);

	}

	void CreateCylinder() {
		float rad = 1;
		float height = 2;
		int num = 20;

		std::vector<float> cyl_vertices;

		std::vector<uint> indices;
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
			if(i+1 == num+1)
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

		m.num_vertexs = cyl_vertices.size()/3;
		m.num_indices = indices.size();

		m.vertexs = new float[m.num_vertexs * 3];
		m.indices = new uint[m.num_indices];
		memcpy(m.vertexs, &cyl_vertices[0], sizeof(float) * m.num_vertexs * 3);
		for (uint i = 0; i <m.num_indices; i++)
		{
			memcpy(&m.indices[i], &indices[i], sizeof(uint));
		}
		glGenBuffers(1, (GLuint*)&(m.id_vertexs));
		glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);

		// Buffer for indices
		glGenBuffers(1, (GLuint*)&m.id_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *m.num_indices, m.indices, GL_STATIC_DRAW);

		App->assimp->mesh_vector->push_back(m);

	}

public:
	Mesh p_mesh;

};

