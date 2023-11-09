#ifndef __MEMMGR_H__
#define __MEMMGR_H__

//+----------------------------------------
//| Memory Manager.
//| Allocates and tracks memory usage, and detects leaks.
//| Version 1.0001
//+----------------------------------------

#include <windows.h>
#include <stdio.h>
#include <string>

#ifndef __ALLOCHEADER_H__
#include "AllocHeader.h"
#endif

#ifndef __MEMHEAP_H__
#include "MemHeap.h"
#endif

#ifndef __MEMPOOL_H__
#include "MemPool.h"
#endif

class IMemoryChunk;

class MemMgr
{
public:
	static void Initialize();
	static void ShutDown();

	static bool CreatePool(size_t sBlockSize, int nNumInitialBlocks, int & index);
	static void DeletePool(int nID);

	static MemHeap* GetHeap();
	static MemPool* GetPool(UINT index);

	static void PrintInfo(FILE* pFile);
	static void PrintInfo(std::string & info);
	static int  ReportMemoryLeaks (FILE* pFile);

private:
	static void DestroyChunk (IMemoryChunk * pHeap);

	const static int MAXPOOLS = 6;

	static MemPool ms_pools[MAXPOOLS];
	static MemHeap ms_heap;
};

#endif
