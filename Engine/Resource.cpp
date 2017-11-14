#include "Resource.h"

Resource::Resource(UUID uid, Resource::Type type)
{


}
bool Resource::LoadToMemory()
{
	if (loaded > 0)
		loaded++;
	else
		loaded = LoadInMemory() ? 1 : 0;

	return loaded > 0;
}

bool Resource::EraseFromMemory()
{
	if (loaded > 1) {
		loaded--;
	}
	else {
		loaded = EraseInMemory() ? 1 : 0;
	}

	return loaded > 0;
}

