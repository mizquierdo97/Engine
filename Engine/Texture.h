#ifndef _TEXTURE
#define _TEXTURE

#include <Windows.h>
#include "Glew/include/glew.h" // extension lib
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Color.h"
#include <string>

typedef unsigned int uint;

class Texture
{
public:
	Texture();
	~Texture();

	bool Create(GLuint* pixels,uint width, uint height);
	void Resize(uint width, uint height);
	void Clear(Color clear_color);
	void Bind();
	void Unbind();
	void Draw();
	GLuint GetTexture() const;
	GLuint GetFBO() const;
	bool UpdateTexture(GLuint* pixels, uint width, uint height);
private:

	void Destroy();

public:
	std::string texture_path;
	int width;
	int height;
private:
	GLuint texture_id;
	GLuint color_texture;
	GLuint depth_texture;

	
};

#endif