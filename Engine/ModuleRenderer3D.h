#ifndef _MODULERENDERER
#define _MODULERENDERER

#include "Module.h"
#include "Globals.h"

#include "Light.h"
#include "MathGeoLib.h"
#include "ModuleAssimp.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <shellapi.h>
#include "Texture.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"
#include "Object.h"
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib  */  
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib  */ 

#include "ShaderProgram.h"

#define MAX_LIGHTS 8

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D( bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	bool Options();
	void Render(ComponentMesh* );
	void RenderMesh(Mesh*);
	void RenderNormals(GameObject*);
	
	void DebugDrawBox(const float3*,Color);
	void DebugDraw(const AABB& aabb, Color color = White, const float4x4& transform = float4x4::identity);

	void UseTexture(uint shader_id = 0, uint i = 0, uint num =0);
	ComponentCamera* GetActiveCamera();
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool render_fill = true;
	bool render_wireframe = false;
	bool debug_draw = true;
	
	uint mTextureID = 0;

	uint DefaultTexture;
	ShaderProgram temp_shader;

	//OPTIONS
	bool texture = true;

};

#endif