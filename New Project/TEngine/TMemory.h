#ifndef __TMEMORY_H__
#define __TMEMORY_H__

//+----------------------------------------
//| Defines TNew and TDelete macros.
//| Used instead of the standard operators, to make use of memory manager.
//| Version 1.000
//+----------------------------------------

#pragma warning (disable : 4291)	// no matching delete

#ifndef __MEMMGR_H__
#include "MemMgr.h"
#endif

//-------------------------------------------------------------------------------
extern void* operator new(size_t size, IMemoryChunk* pChunk, char* file, int line);

#define TNew					::new(MemMgr::GetHeap(), __FILE__, __LINE__)
#define TPoolNew(pool)			::new(MemMgr::GetPool(pool), __FILE__, __LINE__)
#define TSafeDelete(pObject)	if(pObject != NULL) {TDelete(pObject); pObject=NULL;}

//-------------------------------------------------------------------------------
template <class Type>
inline void			
TDelete(Type*& ptr)
{
	ptr->~Type();

	IMemoryChunk::Deallocate(ptr);

	ptr = 0;
};

#endif
