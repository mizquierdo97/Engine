#pragma once

#include "Resource.h"

class ResourceTexture : public Resource {

public :
	ResourceTexture() {};
	ResourceTexture(UUID uid) {
		uuid = uid;
	};
	~ResourceTexture() {};
	bool LoadInMemory() {
		return true;
	};


};