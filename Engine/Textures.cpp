#include "Texture.h"
#include "Application.h"
#include "ModuleWindow.h"

Texture::Texture()
{
	texture_id = 0;
	color_texture = 0;
	depth_texture = 0;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	Destroy();
}

bool Texture::Create(GLuint* pixels, uint width, uint height)
{
	
	//Creates Framebuffer Object
	glGenFramebuffers(1, &texture_id);
	glBindFramebuffer(GL_FRAMEBUFFER, texture_id);

	//Creates Color texture
	glGenTextures(1, &color_texture);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,pixels);

	//Creates Depth Texture
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	//Attach both textures to fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0); //This is optional

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindTexture(GL_TEXTURE_2D,0);
	this->width = width;
	this->height = height;

	//If the fbo is not created returns false
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void Texture::Resize(uint new_width, uint new_height)
{
	//In case you need to resize the texture. Not used right now.
	if (width == new_width && height == new_height) {
		return;
	}
	Destroy();
	Create(nullptr, new_width, new_height);
	width = new_width;
	height = new_height;
	App->renderer3D->OnResize(new_width, new_height);
}

void Texture::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, texture_id);
	glViewport(0, 0, width, height);
}

void Texture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App->window->width, App->window->height);
}

void Texture::Destroy()
{
	if(texture_id)
	glDeleteFramebuffers(1, &texture_id);
	if (color_texture)
	glDeleteTextures(1, &color_texture);
	if (depth_texture)
	glDeleteTextures(1, &depth_texture);
}

void Texture::Clear(Color clear_color)
{
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Texture::Draw()
{
	//Future method to draw all the elements to the texture
}

uint Texture::GetFBO() const
{
 	return texture_id;
}

bool Texture::UpdateTexture(GLuint * pixels, uint width, uint height)
{


	//Creates Framebuffer Object
	if (texture_id == 0 ||color_texture == 0 || depth_texture ==0)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, texture_id);

	//Creates Color texture	
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	//Creates Depth Texture	
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	//Attach both textures to fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0); //This is optional

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->width = width;
	this->height = height;

	//If the fbo is not created returns false
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

uint Texture::GetTexture() const
{
	if(this != nullptr)
	return color_texture;
	return 0;
}
