#pragma once
#include "Object.h"
#include <string>


class Config;

class Resource
{
public:
	enum Type {
		texture,
		mesh,
		audio,
		scene,
		bone,
		animation,
		unknown
	};

public:
	
	Resource(UUID uid, Resource::Type type);
	/*virtual ~Resource();
	Resource::Type GetType() const;
	UUID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint CountReferences() const;
	virtual void Save(Config& config) const;
	virtual void Load(const Config& config);*/
	virtual bool LoadInMemory() = 0;

protected:
	UUID uuid = IID_NULL;
	std::string file;
	std::string exported_file;
	Type type = unknown;
	uint loaded = 0;
};
