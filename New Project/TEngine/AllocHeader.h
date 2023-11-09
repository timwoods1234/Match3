#ifndef __ALLOCHEADER_H__
#define __ALLOCHEADER_H__

//+----------------------------------------
//| Allocation header that precedes all memory allocations.
//| Version 1.000
//+----------------------------------------

#include <windows.h>

class IMemoryChunk;

struct AllocHeader
{
	int           nAllocNum;
	int           nSize;

	IMemoryChunk* pChunk;	// for pooled memory
	int			  nMemIndex;

	AllocHeader * pNext;	// for heaped memory
	AllocHeader * pPrev;

#if defined(DEBUG) | defined(_DEBUG)
	char  file[64];
	DWORD lnum;
#endif
};

#endif
