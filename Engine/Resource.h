#ifndef _RESOURCE
#define _RESOURCE
#include "Object.h"
#include <string>


class Config;

class Resource
{
public:
	enum Type {

		texture,
		mesh,
		shader_program,
		vertex_shader,
		fragment_shader,
		audio,
		scene,
		bone,
		animation,
		unknown
	};

public:
	Resource() {

	};
	Resource(UUID uid, Resource::Type type);
	virtual ~Resource() {};

	/*
	Resource::Type GetType() const;
	UUID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	
	uint CountReferences() const;
	virtual void Save(Config& config) const;
	virtual void Load(const Config& config);*/
	bool LoadToMemory();
	bool EraseFromMemory();
	virtual bool LoadInMemory() = 0;
	virtual bool EraseInMemory() = 0;
	virtual bool UpdateRes() = 0;

public :
	UUID uuid = IID_NULL;
	std::string file;
	std::string exported_file;
	Type type = unknown;
	uint loaded = 0;
	Data meta_data;
};
#endif