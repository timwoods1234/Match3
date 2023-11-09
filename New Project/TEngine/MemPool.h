#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

//+----------------------------------------
//| A contiguous block of memory to be used as a pool.
//| Pool block size can be arbitrary so this can be used for generic allocations.
//| E.g. if you wanted to store allocations of similar size
//| (like a series of classes that share an inheritance tree).
//| Version 1.000
//+----------------------------------------

#ifndef __IMEMORYCHUNK_H__
#include "IMemoryChunk.h"
#endif

class MemPool : public IMemoryChunk
{
public:
	void   Initialize();
	void*  Allocate(size_t bytes, char* file="", int line=0);
	void   FreeMemory(AllocHeader * pHeader);
	void   Activate (int nID, size_t sBlockSize, int nNumBlocks);
	void   Deactivate();
	void   PrintInfo (FILE* pFile, int indentLevel = 0) const;
	int    ReportMemoryLeaks (FILE* pFile);
	bool   IsFull() const { return (m_nInstances == m_numBlocks); }
	size_t GetBlockSize() const { return m_blockSize; }

private:
	BOOL*  pMemInUse;	// flag for each 'block'
	char*  pMemory;
	size_t m_blockSize;
	int    m_numBlocks;
};

#endif
