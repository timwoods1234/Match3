#include "TError.h"
#include "MemPool.h"
#include "MemMgr.h"

//-------------------------------------------------------------------------------
void MemPool::Initialize()
{
	m_bActive        = false;
	m_bytesAllocated = 0;
	m_bytesPeak      = 0;
	m_nInstances	 = 0;
}

//-------------------------------------------------------------------------------
void MemPool::Activate (int nID, size_t sBlockSize, int nNumBlocks)
{
	m_nID			 = nID;
	m_blockSize		 = sBlockSize;
	m_numBlocks		 = nNumBlocks;

	m_bytesAllocated = 0;
	m_bytesPeak      = 0;
	m_bActive        = true;
	m_nInstances	 = 0;

	// Reserve space, all blocks start unused
	pMemory = (char*)malloc(nNumBlocks * (sBlockSize+sizeof(AllocHeader)));
	pMemInUse = (BOOL*)malloc(nNumBlocks * sizeof(BOOL));
	for (int i=0; i<nNumBlocks; i++)
		pMemInUse[i] = false;
}

//-------------------------------------------------------------------------------
void MemPool::Deactivate ()
{
	// free memory
	if (m_bActive)
	{
		free(pMemInUse);
		free(pMemory);
	}

	m_bytesAllocated = 0;
	m_bytesPeak      = 0;
	m_bActive		 = false;
	m_nID			 = 0;
	m_nInstances	 = 0;
}

//-------------------------------------------------------------------------------
void MemPool::PrintInfo (FILE* pFile, int indentLevel) const
{
	fprintf(pFile, "Memory Pool %d, %d allocations (%d bytes)\n", m_nID, m_nInstances, m_bytesAllocated);

	for (int i=0; i<m_numBlocks; i++)
	{
		if (pMemInUse[i])
		{
			int pMemBlock = i * (m_blockSize + sizeof(AllocHeader));
			AllocHeader * pHeader = (AllocHeader*)((char*)pMemory + pMemBlock);

			fprintf(pFile, "\t Block %d, AllocID %d, %s \n",
				i, pHeader->nAllocNum, TraceToSource(pHeader));
		}
	}
}

//-------------------------------------------------------------------------------
void * MemPool::Allocate (size_t bytes, char* file, int line)
{
	size_t nRequestedBytes = m_blockSize+sizeof(AllocHeader);

	if ((bytes+m_bytesAllocated) > (m_blockSize*m_numBlocks))
	{
		TASSERT(false, "MemPool -- not enough free memory");
		return NULL;
	}

	// find empty block
	int i = 0;
	while ((i < m_numBlocks) && pMemInUse[i])
	{
		i++;
	}

	if (i >= m_numBlocks)
	{
		TASSERT(false, "MemPool -- Pool is full");
		return NULL;
	}

	// pMemBlock = start of requested memory block in pMemory
	int pMemBlock = i * (m_blockSize + sizeof(AllocHeader));
	pMemInUse[i] = true;

	AllocHeader * pHeader = (AllocHeader*)((char*)pMemory + pMemBlock);

#if defined(DEBUG) | defined(_DEBUG)
	// track allocation
	strcpy_s(pHeader->file, file);
	pHeader->lnum = line;
#endif

	pHeader->nAllocNum = s_nNextAllocNum++;
	pHeader->nMemIndex = i;
	pHeader->pChunk = this;
	pHeader->nSize = bytes;

	m_bytesAllocated += bytes;

	if (m_bytesAllocated > m_bytesPeak)
		m_bytesPeak = m_bytesAllocated;

	m_nInstances++;

	// return address is just after index marker
	void * pStartMemBlock = (char*)(pMemory + pMemBlock + sizeof(AllocHeader));
	return pStartMemBlock;
}

//-------------------------------------------------------------------------------
void MemPool::FreeMemory(AllocHeader * pHeader)
{
	pMemInUse[pHeader->nMemIndex] = false;

	m_bytesAllocated -= pHeader->nSize;

	m_nInstances--;
}

//-------------------------------------------------------------------------------
int MemPool::ReportMemoryLeaks (FILE* pFile)
{
	int nLeaks = 0;

	for (int i=0; i<m_numBlocks; i++)
	{
		if (pMemInUse[i])
		{
			int pMemBlock = i * (m_blockSize + sizeof(AllocHeader));
			AllocHeader * pHeader = (AllocHeader*)((char*)pMemory + pMemBlock);

			fprintf(pFile, "Leak in Pool %d, block %d, address: 0x%0Xd %s \n", 
				m_nID, i, (char *)pHeader + sizeof(AllocHeader),
				TraceToSource(pHeader));
			nLeaks++;
		}
	}

	return nLeaks;
}
