#include "TError.h"
#include "IMemoryChunk.h"
#include "MemMgr.h"

//-------------------------------------------------------------------------------
int IMemoryChunk::s_nNextAllocNum = 1;

//-------------------------------------------------------------------------------
int IMemoryChunk::GetID() const
{
	return m_nID;
}

//-------------------------------------------------------------------------------
void IMemoryChunk::Deallocate (void * pMem)
{
	AllocHeader * pHeader = (AllocHeader *)((char *)pMem -
		sizeof(AllocHeader));

	pHeader->pChunk->FreeMemory(pHeader);
}

//-------------------------------------------------------------------------------
int IMemoryChunk::GetMemoryBookmark ()
{
	return s_nNextAllocNum;
}

//-------------------------------------------------------------------------------
const char* IMemoryChunk::TraceToSource(AllocHeader* pHeader)
{
#if defined(DEBUG) | defined(_DEBUG)
	static char buffer[512];
	sprintf_s(buffer, 512, "%s:%d", pHeader->file, pHeader->lnum);
	return buffer;
#else
	return "trace unavailable in release";
#endif
}