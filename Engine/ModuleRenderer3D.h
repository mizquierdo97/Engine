#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "ModuleAssimp.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <shellapi.h>
#include "Texture.h"
#include "DevIL\include\il.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib  */  
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib  */ 

#define MAX_LIGHTS 8

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D( bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	bool Options();
	void Render(Mesh mesh);

	bool loadTextureFromFile(char* path);
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height);
	
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	//TESTING MATHGEO
	Sphere* test_sphere1;
	Sphere* test_sphere2;

	
	//
	GLuint gProgramID = 0;
	GLint gVertexPos2DLocation = -1;
	GLuint gVBO = 0;
	GLuint gIBO = 0;

	GLuint vertexbuffer;

	bool render_fill = false;
	bool render_wireframe = true;

	
	uint mTextureID = 0;

	Texture* tex;
};