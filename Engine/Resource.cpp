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
;
