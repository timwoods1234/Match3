#ifndef __MEMHEAP_H__
#define __MEMHEAP_H__

//+----------------------------------------
//| Heaped memory, stored as a linked list of allocations.
//| Version 1.000
//+----------------------------------------

#ifndef __ALLOCAHEADER_H__
#include "AllocHeader.h"
#endif

#ifndef __IMEMORYCHUNK_H__
#include "IMemoryChunk.h"
#endif

class MemHeap : public IMemoryChunk
{
public:
	void  Initialize();
	void* Allocate(size_t bytes, char* file="", int line=0);
	void  FreeMemory(AllocHeader * pHeader);
	void  Activate(int nID);
	void  Deactivate();
	void  PrintInfo(FILE* pFile, int indentLevel = 0) const;
	int   ReportMemoryLeaks(FILE* pFile);

private:
	AllocHeader * m_pHeadAlloc;
};

#endif
