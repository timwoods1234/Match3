#ifndef __IMEMORYCHUNK_H__
#define __IMEMORYCHUNK_H__

//+----------------------------------------
//| Base class for an area of memory.
//| Extended for memory pool and heap.
//| Version 1.000
//+----------------------------------------

#ifndef __ALLOCHEADER_H__
#include "AllocHeader.h"
#endif

#include <stdio.h>

class IMemoryChunk
{
public:
	virtual void* Allocate(size_t bytes, char* file="", int line=0) = 0;
	virtual void  FreeMemory(AllocHeader * pHeader) = 0;
	virtual void  Deactivate () = 0;
	virtual void  PrintInfo (FILE* pFile, int indentLevel = 0) const = 0;
	virtual int   ReportMemoryLeaks (FILE* pFile) = 0;

	static void   Deallocate (void * pMem);	// not virtual
	static int    GetMemoryBookmark ();
	static const  char* TraceToSource(AllocHeader *pHeader);

	bool   IsActive () const { return m_bActive; }
	int    GetID() const;
	size_t GetBytesAllocated() const { return m_bytesAllocated; }

protected:
	bool   m_bActive;
	int	   m_nID;
	size_t m_bytesAllocated;
	size_t m_bytesPeak;
	int    m_nInstances;

	static int s_nNextAllocNum;
};

#endif
