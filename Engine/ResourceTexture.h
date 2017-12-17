#ifndef _RESOURCE_TEXTURE
#define _RESOURCE_TEXTURE
#include "Resource.h"

class ResourceTexture : public Resource {

public :
	ResourceTexture() {};
	ResourceTexture(UUID uid) {
		uuid = uid;
	};
	~ResourceTexture() {
		if(res_tex != nullptr)
		RELEASE(res_tex);
	};
	bool LoadInMemory();
	bool EraseInMemory();
	bool UpdateRes();

public:
	Texture* res_tex = nullptr;
	
};
#endif