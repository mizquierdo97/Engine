
#include <vector>
#include <Windows.h>
#include "Globals.h"
#include "ModuleAssimp.h"
#include "Application.h"
#include "FileSystem.h"
#include "MeshImporter.h"
#include "Object.h"
#include <map>

void CreateBinary(aiScene*, const char*, const char*);
char* LoadBuffer(const char*);
GameObject* CreateObjectFromMesh(char** buffer, GameObject* parent, int* num_childs);
void RecursiveLoad(char** cursor, GameObject* parent);

void GenGLBuffers(Mesh*);
void Recursive(aiNode* root, char** cursor, aiScene* scene, int i,const char* name);
void TransformMeshToBinary(aiNode* root, char** cursor, aiScene* scene, int i,const char* path_name);
uint GetRecursiveSize(aiNode* root, aiScene* scene);


void MeshImporter::ImportMesh(aiScene* scene, const char * directory, const char* name)
{
	if (scene != nullptr && scene->HasMeshes())
	{
		CreateBinary(scene, directory, name);

		LoadGLTextures(scene);
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

	RELEASE_ARRAY(buffer);

}

Mesh MeshImporter::LoadComponentMesh(char* name, uint* ranges)
{
	Mesh m;
	uint bytes;
	std::string temp_name;
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
	temp_name = name;
	//strcpy(m.mesh_path, temp_name.c_str());
	m.mesh_path = (char*)temp_name.c_str();
	GenGLBuffers(&m);
	RELEASE_ARRAY(buffer);
	return m;

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
	Recursive(root, &cursor, scene, 0, name);


	//WRITE FILE
	FILE * pFile;
	final_name = MESHES_PATH;
	final_name += name; final_name += ".bin";
	if (!ExistsFile(final_name.c_str())) {
		pFile = fopen(final_name.c_str(), "wb");
		fwrite(data, sizeof(char), size, pFile);
		fclose(pFile);
		
	}
	if (std::find(App->gui->path_list.begin(), App->gui->path_list.end(), final_name) == App->gui->path_list.end())
		App->gui->path_list.push_back(final_name);

	if (data != nullptr)
		RELEASE_ARRAY(data);

	for (int i = 0; i < scene->mNumMeshes; i++) {

		uint bytes = 0;
		size = 0;
		char* data_mesh = nullptr;

		aiMesh* m;
		m = scene->mMeshes[i];

		//GET SIZE AND ALLOVATE MEMORY
		uint ranges[4] = { m->mNumVertices, m->mNumFaces * 3,m->HasTextureCoords(0),m->HasNormals() };

		size += sizeof(float)* m->mNumVertices * 3  + sizeof(uint);

		if (m->HasFaces()) {
			size += sizeof(uint)* m->mNumFaces * 3;
		}
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
	
			strcpy(mesh_name, name);
			char* num = new char[4];
			itoa(i, num, 10);
			strcat(mesh_name, "_");
			strcat(mesh_name, num);
			RELEASE_ARRAY(num);
			
			bool continue_for= false;
		uint * indices = new uint[m->mNumFaces * 3]; // assume each face is a triangle
		for (uint i = 0; i < m->mNumFaces; ++i)
		{
			if (m->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!"); 
				continue_for = true;
				break;
			}
			else
				memcpy(&indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
		}
		if (continue_for)
			continue;

		bytes = sizeof(uint) *  m->mNumFaces * 3;
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
			RELEASE_ARRAY(texture_coords);
		}

		if (m->HasNormals()) {
			bytes = sizeof(float) *m->mNumVertices * 3;
			memcpy(cursor, m->mNormals, bytes);
			cursor += bytes;
		}

		bytes = sizeof(int);
		int material_index = -1;

		aiString path;
		if (scene->mMaterials[m->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			material_index = m->mMaterialIndex;
		memcpy(cursor, &material_index, bytes);
		cursor += bytes;


		//OK~
		FILE * mesh_pFile;
		std::string final_mesh_name = MESHES_PATH;

		final_mesh_name += mesh_name; final_mesh_name += ".mesh";
		if (!ExistsFile(final_mesh_name.c_str())) {
			pFile = fopen(final_mesh_name.c_str(), "wb");
			fwrite(data_mesh, sizeof(char), size, pFile);
			fclose(pFile);
		}

		RELEASE_ARRAY(indices);
		if (data_mesh != nullptr)
			RELEASE_ARRAY(data_mesh);
	}

	

}

int MeshImporter::LoadGLTextures(const aiScene* scene)
{
	ILboolean success;

	/* initialization of DevIL */
	ilInit();

	/* scan scene's materials for textures */
	for (unsigned int m = 0; m<scene->mNumMaterials; ++m)
	{
		int texIndex = 0;
		aiString path;  // filename

		if (scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			//while (texFound == AI_SUCCESS) {
				//fill map with textures, OpenGL image ids set to 0
			std::string file_name = ASSETS_PATH;
			file_name += GetFileNameExtension(path.data);
			textureIdMap[m] = file_name;
			// more textures?
			//texIndex++;
			//texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		}
	}

	return true;
}

//OK
void Recursive(aiNode* root, char** cursor, aiScene* scene, int i,const char* name) {

	//Create Mesh from children i
	TransformMeshToBinary(root, cursor, scene, i,name);
	//Create mesh from son of 1
	for (int n = 0; n < root->mNumChildren; n++) {
		Recursive(root->mChildren[n], cursor, scene, n,name);
	}
}


void TransformMeshToBinary(aiNode* root, char** cursor, aiScene* scene, int i, const char* path_name) {


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
			
			for (uint i = 0; i < m->mNumFaces; ++i)
			{
				if (m->mFaces[i].mNumIndices != 3) {
					LOG("WARNING, geometry face with != 3 indices!");
					ranges[1] = 0;
				}
				
			}
				strcpy(mesh_name,path_name);
				char* num = new char[4];
				itoa(root->mMeshes[num_mesh], num, 10);
				strcat(mesh_name, "_");
				strcat(mesh_name, num);
				RELEASE_ARRAY(num);
			
		}	
		else {
			strcpy(mesh_name, "root_");
			strcat(mesh_name, path_name);
		}
		root->mTransformation.Decompose(scaling, rotation, translation);
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
void RecursiveLoad(char** cursor, GameObject* parent) {

	int num_childs;
	parent = CreateObjectFromMesh(&cursor[0], parent, &num_childs);

	for (int i = 0; i < num_childs; i++) {
		RecursiveLoad(&cursor[0], parent);
	}
}


GameObject* CreateObjectFromMesh(char** cursor, GameObject* parent, int* num_childs) {

	uint ranges[4];
	Mesh m;
	aiVector3D translation;
	aiQuaternion rotation;
	aiVector3D scaling;
	GameObject* temp_obj;
	int material_index = -1;
	int num_meshes = 0;
	int num = 0;
	uint bytes = sizeof(uint);
	memcpy(&num_meshes, cursor[0], bytes);
	cursor[0] += bytes;
	std::string temp_name;
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
		
		if (ranges[0] != 0 && ranges[1] != 0) {

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

			bytes = sizeof(int);
			
			memcpy(&material_index, cursor_mesh, bytes);
			cursor_mesh += bytes;
				
			GenGLBuffers(&m);
			RELEASE_ARRAY(buffer);
		}
	
		temp_name = name;
		m.mesh_path = (char*)temp_name.c_str();
	
	

		AABB* temp = new AABB();
		temp->SetFrom((float3*)m.vertexs, m.num_vertexs);
		

		temp_obj = new GameObject();
		temp_obj->obj_parent = parent;
		
		if(m.id_vertexs!=0)
		temp_obj->AddComponentMesh(m);
		temp_obj->AddComponentTransform(translation, rotation, scaling);
		temp_obj->SetGlobalBox(*temp);
		UpdateAABB(temp_obj);
		temp_obj->SetName((char*)m.mesh_path.c_str());	

		Texture* temp_text = new Texture();
		if (material_index != -1) {
			/*std::map<GLuint, std::string>::iterator pos */ auto temp = App->filesystem->mesh_importer->textureIdMap.find(material_index);
			std::string texture_path = MESHES_PATH + GetFileName(temp->second) + ".dds";

			UUID obj_uuid = App->resources->FindImported(texture_path.c_str());

			if(!App->resources->Get(obj_uuid)->LoadToMemory())
			if (App->filesystem->image_importer->loadTextureFromFile((char*)texture_path.c_str(), &temp_text)) {
				LOG("TEXTURE_LOADED");
				
			}
			
			temp_obj->AddComponentMaterial(obj_uuid);
		}

		//FINALLY CREATE OBJECT
		CreateObject(temp_obj);

		//FREE MEMORY
		//RELEASE_ARRAY( name);
		//RELEASE_ARRAY( m.indices);
		//RELEASE_ARRAY( m.vertexs);
		//RELEASE_ARRAY( m.norms);
		//RELEASE_ARRAY( m.texture_coords);
	
		RELEASE(temp);
		
		
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