#include "TError.h"
#include "MemHeap.h"
#include "MemMgr.h"

//-------------------------------------------------------------------------------
void MemHeap::Initialize()
{
	m_bActive        = false;
	m_bytesAllocated = 0;
	m_bytesPeak      = 0;
	m_nInstances	 = 0;
	m_pHeadAlloc	 = NULL;
}

//-------------------------------------------------------------------------------
void MemHeap::Activate (int nID)
{
	m_nID			 = nID;
	m_bytesAllocated = 0;
	m_bytesPeak      = 0;
	m_bActive        = true;
	m_nInstances     = 0;
}

//-------------------------------------------------------------------------------
void MemHeap::Deactivate ()
{
	m_bytesAllocated = 0;
	m_bytesPeak      = 0;
	m_bActive		 = false;
	m_nID			 = 0;
	m_nInstances     = 0;
}

//-------------------------------------------------------------------------------
void MemHeap::PrintInfo (FILE* pFile, int indentLevel) const
{
	fprintf(pFile, "Heaped Memory (%d bytes)\n", m_bytesAllocated);

	AllocHeader *pHeader = m_pHeadAlloc;
	while (NULL != pHeader)
	{
		fprintf(pFile, "\t AllocID %d, %s (%d bytes)\n", pHeader->nAllocNum,
			TraceToSource(pHeader), pHeader->nSize);

		pHeader = pHeader->pNext;
	}
}

//-------------------------------------------------------------------------------
void * MemHeap::Allocate(size_t bytes, char* file, int line)
{
	size_t nRequestedBytes = bytes + sizeof(AllocHeader);
	char * pMem = (char *)malloc (nRequestedBytes);
	if (!pMem)
	{
		TASSERT(false, "MemHeap -- allocation failed");
		return NULL;
	}

	AllocHeader * pHeader = (AllocHeader *)pMem;

#if defined(DEBUG) | defined(_DEBUG)
	// track allocation
	strcpy_s(pHeader->file, 64, file);
	pHeader->lnum = line;
#endif

	pHeader->pChunk		= this;
	pHeader->nSize		= bytes;
	pHeader->pNext		= m_pHeadAlloc;
	pHeader->pPrev		= NULL;
	pHeader->nAllocNum  = s_nNextAllocNum++;

	if (m_pHeadAlloc != NULL)
		m_pHeadAlloc->pPrev = pHeader;
	m_pHeadAlloc = pHeader;

	m_bytesAllocated += bytes;
	if (m_bytesAllocated > m_bytesPeak)
		m_bytesPeak = m_bytesAllocated;
	m_nInstances++;

	void * pStartMemBlock = (char*)(pMem + sizeof(AllocHeader));
	return pStartMemBlock; 
}

//-------------------------------------------------------------------------------
void MemHeap::FreeMemory(AllocHeader * pHeader)
{
	if (pHeader->pPrev == NULL)
	{
		m_pHeadAlloc = pHeader->pNext;
	}        
	else
		pHeader->pPrev->pNext = pHeader->pNext;

	if (pHeader->pNext != NULL)
		pHeader->pNext->pPrev = pHeader->pPrev;   

	m_bytesAllocated -= pHeader->nSize;
	m_nInstances--;
	free (pHeader);
}

//-------------------------------------------------------------------------------
int MemHeap::ReportMemoryLeaks (FILE* pFile)
{
	int nLeaks = 0;

	AllocHeader * pHeader = m_pHeadAlloc;
	while (pHeader != NULL) 
	{
		fprintf(pFile, "Leak in MemHeap. Size: %d, address: 0x%0Xd %s \n", 
			pHeader->nSize, (char *)pHeader + sizeof(AllocHeader),
			TraceToSource(pHeader));
		nLeaks++;

		pHeader = pHeader->pNext;
	}

	return nLeaks;
}
