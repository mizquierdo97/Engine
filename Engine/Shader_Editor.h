#ifndef SHADER_EDITOR_TEXT
#define SHADER_EDITOR_TEXT

#include "TextEditor.h"

class Shader_Editor
{
public:
	Shader_Editor();
	~Shader_Editor();


	void Enable(bool visible); 
	TextEditor text_shader_editor;

};


#endif // !SHADER_EDITOR_TEXT
