#include <string>
#include "TMemory.h"

//-------------------------------------------------------------------------------
void * operator new(size_t size, IMemoryChunk* pChunk, char* file, int line)
{
#if defined(DEBUG) | defined(_DEBUG)
	// cast to std::string to remove path (just want filename)
	std::string strfile = file;
	strfile = strfile.substr(strfile.rfind("\\")+1);

	return pChunk->Allocate(size, const_cast<char*>(strfile.c_str()), line);
#else
	return pChunk->Allocate(size);
#endif
}
