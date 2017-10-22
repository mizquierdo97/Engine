
#include <vector>
#include <Windows.h>
#include "Globals.h"
#include "ModuleAssimp.h"
#include "Application.h"
#include "FileSystem.h"
#include "MeshImporter.h"

void CreateBinary(aiScene*, const char*, const char*);
char* LoadBuffer(const char*);
Object* CreateObjectFromMesh(char** buffer, Object* parent, int* num_childs);
void RecursiveLoad(char** cursor, Object* parent);

void GenGLBuffers(Mesh*);
void Recursive(aiNode* root, char** cursor, aiScene* scene, int i);
void TransformMeshToBinary(aiNode* root, char** cursor, aiScene* scene, int i);
uint GetRecursiveSize(aiNode* root, aiScene* scene);


void MeshImporter::ImportMesh(aiScene* scene, const char * directory, const char* name)
{
	if (scene != nullptr && scene->HasMeshes())
	{
		CreateBinary(scene, directory, name);
	}
	else {
		LOG("Can't open the file: %s", directory);
	}
}


void MeshImporter::LoadMesh(const char * path)
{
	char* buffer = LoadBuffer(path);
	/* the whole file is now loaded in the memory buffer. */
	char* cursor = buffer;
	// amount of indices / vertices / colors / normals / texture_coords
	RecursiveLoad(&cursor, nullptr);

	delete[] buffer;

}

//OK


uint GetSize(aiScene* scene) {

	uint size = 0;
	aiMesh* m;
	aiNode* root = scene->mRootNode;

	size = GetRecursiveSize(root, scene);
	//size += scene->mNumMeshes * sizeof(uint);

	return size;
};

uint GetRecursiveSize(aiNode* root, aiScene* scene) {

	aiMesh* m;
	uint size = 0;
	int num = 0;

	do {
		
		uint ranges[4] = { 0,0,0,0 };
		size += sizeof(ranges);
		

		size += sizeof(aiVector3D) * 2 + sizeof(aiQuaternion);
		size += sizeof(char) * 50;
		size += sizeof(uint);
		

		for (int i = 0; i < root->mNumChildren; i++) {
			size += GetRecursiveSize(root->mChildren[i], scene);
		}
		num++;
	}
	while (num < root->mNumMeshes);

	size += sizeof(uint);

	return size;
}

//OK


void CreateBinary(aiScene* scene, const char * directory, const char* name) {

	std::string final_name;
	uint num_meshes[1] = { scene->mNumMeshes };

	//ALLOCATE MEMORY
	char* data = nullptr;
	uint size = GetSize(scene);
	data = new char[size];
	char* cursor = data;

	aiNode* root = scene->mRootNode;
	Recursive(root, &cursor, scene, 0);


	//WRITE FILE
	FILE * pFile;
	final_name = MESHES_PATH;
	final_name += name; final_name += ".bin";
	pFile = fopen(final_name.c_str(), "wb");
	fwrite(data, sizeof(char), size, pFile);
	fclose(pFile);
	App->gui->path_list->push_back(final_name);

	if (data != nullptr)
		delete[] data;

	for (int i = 0; i < scene->mNumMeshes; i++) {

		uint bytes = 0;
		size = 0;
		char* data_mesh = nullptr;

		aiMesh* m;
		m = scene->mMeshes[i];

		//GET SIZE AND ALLOVATE MEMORY
		uint ranges[4] = { m->mNumVertices, m->mNumFaces * 3,m->HasTextureCoords(0),m->HasNormals() };

		size += sizeof(float)* m->mNumVertices * 3 + sizeof(uint)* m->mNumFaces * 3;
		if (m->HasTextureCoords(0)) {
			size += sizeof(float)*m->mNumVertices * 2;
		}
		if (m->HasNormals()) {
			size += sizeof(float)*m->mNumVertices * 3;
		}
		data_mesh = new char[size];
		cursor = data_mesh;
		//

		char mesh_name[50] = { 0 };
		if (strcmp(m->mName.C_Str(), "")) {
			strcpy(mesh_name, (char*)m->mName.C_Str());
		}
		else {
			strcpy(mesh_name, (char*)m->mName.C_Str());
			char* num = new char[4];
			itoa(i, num, 10);
			strcat(mesh_name, num);
			delete[] num;
		}


		bytes = sizeof(uint) *  m->mNumFaces * 3;

		uint * indices = new uint[m->mNumFaces * 3]; // assume each face is a triangle
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

		bytes = sizeof(float) * m->mNumVertices * 3;
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
			delete[] texture_coords;
		}

		if (m->HasNormals()) {
			bytes = sizeof(float) *m->mNumVertices * 3;
			memcpy(cursor, m->mNormals, bytes);
			cursor += bytes;
		}


		//OK~
		FILE * mesh_pFile;
		std::string final_mesh_name = MESHES_PATH;

		final_mesh_name += mesh_name; final_mesh_name += ".mesh";
		pFile = fopen(final_mesh_name.c_str(), "wb");
		fwrite(data_mesh, sizeof(char), size, pFile);
		fclose(pFile);

		delete[] indices;
		if (data_mesh != nullptr)
			delete[] data_mesh;
	}
}

//OK
void Recursive(aiNode* root, char** cursor, aiScene* scene, int i) {

	//Create Mesh from children i
	TransformMeshToBinary(root, cursor, scene, i);
	//Create mesh from son of 1
	for (int n = 0; n < root->mNumChildren; n++) {
		Recursive(root->mChildren[n], cursor, scene, n);
	}
}


void TransformMeshToBinary(aiNode* root, char** cursor, aiScene* scene, int i) {


	aiMesh* m;
	uint bytes = 0;
	static int n = 0;
	char mesh_name[50] = { 0 };
	aiVector3D translation = { 0,0,0 };
	aiVector3D scaling = { 1,1,1 };
	aiQuaternion rotation = { 0,0,0,0 };
	uint ranges[4] = { 0,0,0,0 };
	int num_mesh = 0;

	bytes = sizeof(uint);
	memcpy(cursor[0], &root->mNumMeshes, bytes);
	cursor[0] += bytes;

	do {
		
		if (root->mNumMeshes > 0) {
			m = scene->mMeshes[root->mMeshes[num_mesh]];
			uint temp[4] = { m->mNumVertices, m->mNumFaces * 3,m->HasTextureCoords(0),m->HasNormals() };
			memcpy(ranges, temp, sizeof(ranges));
			if (strcmp(m->mName.C_Str(), ""))
				strcpy(mesh_name, (char*)m->mName.C_Str());

			else {
				strcpy(mesh_name, (char*)m->mName.C_Str());
				char* num = new char[4];
				itoa(root->mMeshes[num_mesh], num, 10);
				strcat(mesh_name, num);
				delete[] num;
			}

			root->mTransformation.Decompose(scaling, rotation, translation);
		}		

		//HEADER
		bytes = sizeof(ranges);
		memcpy(cursor[0], ranges, bytes);
		cursor[0] += bytes;

		//NAME
		bytes = sizeof(char) * 50;
		memcpy(cursor[0], mesh_name, bytes);
		cursor[0] += bytes;

		//TRANSFORMATION
		bytes = sizeof(aiVector3D);
		memcpy(cursor[0], &translation, bytes);
		cursor[0] += bytes;

		bytes = sizeof(aiQuaternion);
		memcpy(cursor[0], &rotation, bytes);
		cursor[0] += bytes;

		bytes = sizeof(aiVector3D);
		memcpy(cursor[0], &scaling, bytes);
		cursor[0] += bytes;

		//NUMCHILDS
		bytes = sizeof(uint);
		memcpy(cursor[0], &root->mNumChildren, bytes);
		cursor[0] += bytes;


		num_mesh++;
	} while (num_mesh < root->mNumMeshes);
};



//OK
char* LoadBuffer(const char* path) {
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

	fclose(pFile);

	return buffer;
}

//OK
void RecursiveLoad(char** cursor, Object* parent) {

	int num_childs;
	parent = CreateObjectFromMesh(&cursor[0], parent, &num_childs);

	for (int i = 0; i < num_childs; i++) {
		RecursiveLoad(&cursor[0], parent);
	}
}


Object* CreateObjectFromMesh(char** cursor, Object* parent, int* num_childs) {

	uint ranges[4];
	Mesh m;
	aiVector3D translation;
	aiQuaternion rotation;
	aiVector3D scaling;
	Object* temp_obj;
	int num_meshes = 0;
	int num = 0;
	uint bytes = sizeof(uint);
	memcpy(&num_meshes, cursor[0], bytes);
	cursor[0] += bytes;
	
	do{


		bytes = sizeof(ranges);
		memcpy(ranges, cursor[0], bytes);
		cursor[0] += bytes;

		char* name = new char[50];
		bytes = sizeof(char) * 50;
		memcpy(name, cursor[0], bytes);
		cursor[0] += bytes;

		bytes = sizeof(aiVector3D);
		memcpy(&translation, cursor[0], bytes);
		cursor[0] += bytes;

		bytes = sizeof(aiQuaternion);
		memcpy(&rotation, cursor[0], bytes);
		cursor[0] += bytes;

		bytes = sizeof(aiVector3D);
		memcpy(&scaling, cursor[0], bytes);
		cursor[0] += bytes;

		bytes = sizeof(uint);
		memcpy(num_childs, cursor[0], bytes);
		cursor[0] += bytes;




		if (ranges[0] != 0) {

			char path[80] = "Library/Meshes/";
			strcat(path, name);
			strcat(path, ".mesh");

			char * buffer = LoadBuffer(path);
			char* cursor_mesh = buffer;

			m.num_vertexs = ranges[0];
			m.num_indices = ranges[1];

			bytes = sizeof(uint) * m.num_indices;
			m.indices = new uint[m.num_indices];
			memcpy(m.indices, cursor_mesh, bytes);
			cursor_mesh += bytes;

			bytes = sizeof(float) * m.num_vertexs * 3;
			m.vertexs = new float[m.num_vertexs * 3];
			memcpy(m.vertexs, cursor_mesh, bytes);
			cursor_mesh += bytes;

			if (ranges[2]) {
				bytes = sizeof(float) * m.num_vertexs * 2;
				m.texture_coords = new float[m.num_vertexs * 2];
				memcpy(m.texture_coords, cursor_mesh, bytes);
				cursor_mesh += bytes;
			}

			if (ranges[3]) {
				bytes = sizeof(float) * m.num_vertexs * 3;
				m.norms = new float[m.num_vertexs * 3];
				memcpy(m.norms, cursor_mesh, bytes);
				cursor_mesh += bytes;
			}


			GenGLBuffers(&m);
			delete[] buffer;

		}

		AABB* temp = new AABB();
		temp->SetFrom((vec*)m.vertexs, m.num_vertexs);
		m.bounding_box = *temp;

		temp_obj = new Object();
		
		temp_obj->AddComponentMesh(m);
		temp_obj->AddComponentTransform(translation, rotation, scaling);
		/*Texture* text;
		App->renderer3D->loadTextureFromFile("Library/Meshes/image.dds", &text);
		temp_obj->AddComponentMaterial(text);*/
		temp_obj->obj_parent = parent;

		//FREE MEMORY
		delete[] name;
		delete[] m.indices;
		delete[] m.vertexs;
		delete[] m.norms;
		delete[] m.texture_coords;
	


		if (parent != nullptr) {
			parent->obj_childs.push_back(temp_obj);
		}
		else {
			App->world->obj_vector.push_back(temp_obj);
		}
		num++;
	}while (num<num_meshes);
	return temp_obj;

}

void GenGLBuffers(Mesh* m) {

	glGenBuffers(1, (GLuint*)&(m->id_vertexs));
	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertexs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertexs * 3, m->vertexs, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&m->id_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *m->num_indices, m->indices, GL_STATIC_DRAW);
	LOG("Faces buffer created sucessfully");

	glGenBuffers(1, (GLuint*)&m->id_textures);
	glBindBuffer(GL_ARRAY_BUFFER, m->id_textures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertexs * 2, &m->texture_coords[0], GL_STATIC_DRAW);
	LOG("Texture coords: buffer created sucessfully");


	glGenBuffers(1, (GLuint*)&(m->id_norms));
	glBindBuffer(GL_ARRAY_BUFFER, m->id_norms);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertexs * 3, m->norms, GL_STATIC_DRAW);
	LOG("Normals buffer created sucessfully");


}