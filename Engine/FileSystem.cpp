#include "FileSystem.h"
#include <vector>
#include <Windows.h>
#include "Globals.h"
#include "ModuleAssimp.h"
#include "Application.h"
void FileSystem::InitFileSystem()
{
	if (CreateDirectory(ASSETS_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		
	}
	if (CreateDirectory(LIBRARY_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	if (CreateDirectory(MESHES_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	if (CreateDirectory(MATERIALS_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
}

void FileSystem::ImportMesh(aiMesh* m, const char * directory, const char* name)
{

	uint ranges[4] = { m->mNumVertices, m->mNumFaces*3,m->HasTextureCoords(0),m->HasNormals() };
	
	uint size = sizeof(ranges) + sizeof(float)* m->mNumVertices * 3 + sizeof(uint)* m->mNumFaces*3;
	if (m->HasTextureCoords(0)) {
		size += sizeof(float)*m->mNumVertices * 2;
	}
	if (m->HasNormals()) {
		size += sizeof(float)*m->mNumVertices * 3;
	}
	
	char* data = new char[size];
	char* cursor = data;
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(uint) *  m->mNumFaces * 3;
	
	
	uint * indices = new uint[m->mNumFaces*3]; // assume each face is a triangle
	for (uint i = 0; i < m->mNumFaces; ++i)
	{
		if (m->mFaces[i].mNumIndices != 3) {
			LOG("WARNING, geometry face with != 3 indices!");
		}
		else
			memcpy(&indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
	}
	memcpy(cursor, indices, bytes);
	cursor += bytes;


	bytes = sizeof(float) * m->mNumVertices *3;
	memcpy(cursor, m->mVertices, bytes);
	cursor += bytes;
	
	if (m->HasTextureCoords(0)) {
		float* texture_coords = new float[m->mNumVertices * 2];
		for (unsigned int k = 0; k < m->mNumVertices; ++k) {

			texture_coords[k * 2] = m->mTextureCoords[0][k].x;
			texture_coords[k * 2 + 1] = m->mTextureCoords[0][k].y;

		}
		bytes = sizeof(float) * m->mNumVertices * 2;
		memcpy(cursor, texture_coords, bytes);
		cursor += bytes;
	}

	if (m->HasNormals()) {
		bytes = sizeof(float) *m->mNumVertices * 3;
		memcpy(cursor, m->mNormals, bytes);
		cursor += bytes;
	}

	FILE * pFile;
	std::string final_name = MESHES_PATH;
	final_name += name; final_name += ".bin";
	pFile = fopen(final_name.c_str(), "wb");
	fwrite(data, sizeof(char), size, pFile);
	fclose(pFile);
	
	App->gui->path_list->push_back(final_name);
	

	//LoadMesh("myfile.bin");
}

void FileSystem::LoadMesh(const char * path)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen(path, "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = new char[lSize];// (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }
	uint total_size =0;
	/* the whole file is now loaded in the memory buffer. */
	char* cursor = buffer;
	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[4];
	uint bytes = sizeof(ranges);
	total_size += bytes;
	memcpy(ranges, cursor, bytes);
	Mesh m;

	m.num_vertexs = ranges[0];
	m.num_indices = ranges[1];
	cursor += bytes;


	bytes = sizeof(uint) * m.num_indices;
	total_size += bytes;
	m.indices = new uint[m.num_indices];
	memcpy(m.indices, cursor, bytes);
	
	cursor += bytes;
	
	bytes = sizeof(float) * m.num_vertexs *3 ;
	total_size += bytes;
	m.vertexs = new float[m.num_vertexs *3];
	memcpy(m.vertexs, cursor, bytes);
	cursor += bytes;

	if (ranges[2]) {
		bytes = sizeof(float) * m.num_vertexs * 2;
		total_size += bytes;
		m.texture_coords = new float[m.num_vertexs * 2];
		memcpy(m.texture_coords, cursor, bytes);
		cursor += bytes;
	}

	if (ranges[3]) {
		bytes = sizeof(float) * m.num_vertexs * 3;
		total_size += bytes;
		m.norms = new float[m.num_vertexs * 3];
		memcpy(m.norms, cursor, bytes);
	}

	glGenBuffers(1, (GLuint*)&(m.id_vertexs));
	glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&m.id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *m.num_indices, m.indices, GL_STATIC_DRAW);
	LOG("Faces buffer created sucessfully");


	glGenBuffers(1, (GLuint*)&(m.id_norms));
	glBindBuffer(GL_ARRAY_BUFFER, m.id_norms);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, m.norms, GL_STATIC_DRAW);
	LOG("Normals buffer created sucessfully");

	glGenBuffers(1, (GLuint*) &(m.id_textures));
	glBindBuffer(GL_ARRAY_BUFFER, m.id_textures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 2, &m.texture_coords, GL_STATIC_DRAW);
	LOG("Texture coords: buffer created sucessfully");


	AABB* temp = new AABB();
	temp->SetFrom((vec*)m.vertexs, m.num_vertexs);
	m.bounding_box = *temp;

	
	Object* temp_obj = new Object();
	temp_obj->bb_mesh = CreateAABB(*temp);
	temp_obj->AddComponentMesh(m);
	//temp_obj->AddComponentMaterial(temp_tex);
	//temp_obj->obj_mesh = m;
	temp_obj->obj_id = App->world->obj_vector.size();
	//temp_obj->obj_text = temp_tex;
	App->world->obj_vector.push_back(temp_obj);


	// terminate
	fclose(pFile);
	free(buffer);

}


