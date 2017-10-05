#include "Globals.h"
#include "Application.h"
#include "ModuleAssimp.h"
#include "Primitives.h"

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
	mesh_vector = new std::vector<Mesh>;



	
	
	
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
	LOG("Start Exporting");
	const aiScene* scene;
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++) {

			aiMesh* new_mesh = scene->mMeshes[i];

			Mesh m;;
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
			}

			if (new_mesh->HasNormals()) {
				m.num_norms = new_mesh->mNumVertices;
				m.norms = new float[m.num_norms * 3];
				memcpy(m.norms, new_mesh->mNormals, sizeof(float) * m.num_norms * 3);

				glGenBuffers(1, (GLuint*)&(m.id_norms));
				glBindBuffer(GL_ARRAY_BUFFER, m.id_norms);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_norms * 3, m.norms, GL_STATIC_DRAW);

			}

			if (new_mesh->HasVertexColors(0)) {
				m.colors = new float[m.num_vertexs * 3];
				memcpy(m.colors, new_mesh->mColors, sizeof(float) * m.num_vertexs * 3);

				glGenBuffers(1, (GLuint*) &(m.id_colors));
				glBindBuffer(GL_ARRAY_BUFFER, m.id_colors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, m.colors, GL_STATIC_DRAW);
			}

			if (new_mesh->HasTextureCoords(0)) {

				m.texture_coords = new float[m.num_vertexs * 2];
				for (unsigned int k = 0; k < new_mesh->mNumVertices; ++k) {

					m.texture_coords[k * 2] = new_mesh->mTextureCoords[0][k].x;
					m.texture_coords[k * 2 + 1] = new_mesh->mTextureCoords[0][k].y;

				}
				//m.texture_coords = new float[m.num_vertexs * 3];
				//memcpy(m.texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * m.num_vertexs * 3);

				glGenBuffers(1, (GLuint*) &(m.id_textures));
				glBindBuffer(GL_ARRAY_BUFFER, m.id_textures);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertexs * 3, &m.texture_coords[0], GL_STATIC_DRAW);
			}
			
		

			// Buffer for indices
		

			mesh_vector->push_back(m);
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
		}

			aiReleaseImport(scene);
		
	}
	else
		LOG("Error loading scene");


}
