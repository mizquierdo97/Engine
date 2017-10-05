#pragma once

#include <Windows.h>
#include "Glew/include/glew.h" // extension lib
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Color.h"

typedef unsigned int uint;

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	bool Create(uint width, uint height);
	void Resize(uint width, uint height);
	void Clear(Color clear_color);
	void Bind();
	void Unbind();
	void Draw();
	GLuint GetTexture() const;

private:
	void Destroy();

private:
	GLuint fbo;
	GLuint color_texture;
	GLuint depth_texture;
	int width;
	int height;
};

