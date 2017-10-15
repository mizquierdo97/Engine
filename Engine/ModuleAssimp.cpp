#include "Globals.h"
#include "Application.h"
#include "ModuleAssimp.h"

ModuleAssimp::ModuleAssimp(bool start_enabled) : Module(start_enabled)
{
}

ModuleAssimp::~ModuleAssimp()
{
}

bool ModuleAssimp::Init()
{	
	return true;
}

bool ModuleAssimp::Start()
{
	return true;
}

update_status ModuleAssimp::PreUpdate(float dt)
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return UPDATE_CONTINUE;
}

update_status ModuleAssimp::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAssimp::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleAssimp::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

void ModuleAssimp::ImportMesh(char * path)
{
	

	//CLEAN LAST OBJ VECTOR
	std::vector<Object*>::iterator it = App->world->obj_vector.begin();
	if(App->world->obj_vector.size())
		LOG("Cleaning Last FBX mesh")
	while (it != App->world->obj_vector.end()) {
		delete (*it)->obj_mesh.vertexs;
		delete (*it)->obj_mesh.indices;
		delete (*it)->obj_mesh.norms;
		delete (*it)->obj_mesh.texture_coords;
		delete (*it);
		it++;
	}
	App->world->obj_vector.clear();
	

	LOG("Start Exporting");

	const aiScene* scene;
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
		
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++) {

			aiMesh* new_mesh = scene->mMeshes[i];
			Texture* temp_tex = nullptr;
			Mesh m;
			m.num_vertexs = new_mesh->mNumVertices;
			m.vertexs = new float[m.num_vertexs * 3];
			memcpy(m.vertexs, new_mesh->mVertices, sizeof(float) * m.num_vertexs * 3);
			LOG("New mesh with %d vertices", m.num_vertexs);

			glGenBuffers(1, (GLuint*)&(m.id_vertexs));
			glBindBuffer(GL_ARRAY_BUFFER, m.id_vertexs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, m.vertexs, GL_STATIC_DRAW);


			if (new_mesh->HasFaces())
			{
				m.num_indices = new_mesh->mNumFaces * 3;
				m.indices = new uint[m.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

				glGenBuffers(1, (GLuint*)&m.id_indices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *m.num_indices, m.indices, GL_STATIC_DRAW);
				LOG("Faces buffer created sucessfully");
			}
			else {
				LOG("This mesh dont have indices buffer");
			}

			if (new_mesh->HasNormals()) {
				m.num_norms = new_mesh->mNumVertices;
				m.norms = new float[m.num_norms * 3];
				memcpy(m.norms, new_mesh->mNormals, sizeof(float) * m.num_norms * 3);

				glGenBuffers(1, (GLuint*)&(m.id_norms));
				glBindBuffer(GL_ARRAY_BUFFER, m.id_norms);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_norms * 3, m.norms, GL_STATIC_DRAW);
				LOG("Normals buffer created sucessfully");

			}
			else {
				LOG("This mesh dont have normal buffer");
			}

			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;

			for (int i = 0; i < scene->mRootNode->mNumChildren; i++) {
				scene->mRootNode->mChildren[i]->mTransformation.Decompose(scaling, rotation, translation);
				m.translation.Set(translation.x, translation.y, translation.z);
				m.scale.Set(scaling.x, scaling.y, scaling.z);
				m.rotation.Set(rotation.x, rotation.y, rotation.z, rotation.w);
			}

			if (new_mesh->HasTextureCoords(0)) {

				m.texture_coords = new float[m.num_vertexs * 2];
				for (unsigned int k = 0; k < new_mesh->mNumVertices; ++k) {

					m.texture_coords[k * 2] = new_mesh->mTextureCoords[0][k].x;
					m.texture_coords[k * 2 + 1] = new_mesh->mTextureCoords[0][k].y;

				}
			
				glGenBuffers(1, (GLuint*) &(m.id_textures));
				glBindBuffer(GL_ARRAY_BUFFER, m.id_textures);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 2, &m.texture_coords[0], GL_STATIC_DRAW);
				LOG("Texture coords: buffer created sucessfully");
			}
			else {
				LOG("This mesh dont have texture coords buffer");
			}
			if (scene->HasMaterials()) {

				aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
				
				aiString tex_path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &tex_path);

				std::string final_path = path;
				while (final_path.back() != '\\')
				{
					final_path.pop_back();
				}
				final_path += tex_path.C_Str();
				//App->gui->path_list->push_back(final_path);
				App->renderer3D->loadTextureFromFile((char*)final_path.c_str(), &temp_tex);

			}
			
			AABB* temp = new AABB();
			temp->SetFrom((vec*)new_mesh->mVertices,m.num_vertexs);
			m.bounding_box = *temp;
			
			CreateAABB(*temp);
			Object* temp_obj = new Object();
			temp_obj->obj_mesh = m;
			temp_obj->obj_id = App->world->obj_vector.size();
			temp_obj->obj_text = temp_tex;
			App->world->obj_vector.push_back(temp_obj);
		}
			//RELEASE SCENE
			aiReleaseImport(scene);
	}
	else {
		LOG("Can't open the file: %s", path);
	}
}
