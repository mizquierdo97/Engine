#ifndef _CONFIG
#define _CONFIG


#include "Globals.h"
#include "Json\parson.h"

#define FILE_NAME "config.json"
class Config
{
public:
	Config();

	Config::Config(JSON_Object* section) : root_object(section)
	{}
	~Config();


	void Init();
	void Load();
	void Save()const;

public:
	JSON_Object*	root_object;
	JSON_Value*		root_value;

	char* file_name = FILE_NAME;
};

#endif