#include "TError.h"
#include "MemMgr.h"

//-------------------------------------------------------------------------------
MemPool MemMgr::ms_pools[MAXPOOLS];
MemHeap MemMgr::ms_heap;

//-------------------------------------------------------------------------------
void MemMgr::Initialize()
{   
	// init pools (dont activate them until requested - so no memory is allocated here)
	for (int i=0; i < MAXPOOLS; ++i)
		ms_pools[i].Initialize();

	// init heap
	ms_heap.Initialize();
	ms_heap.Activate(1);
}

//-------------------------------------------------------------------------------
bool MemMgr::CreatePool(size_t sBlockSize, int nNumInitialBlocks, int & index)
{
	for (int i=0; i < MAXPOOLS; ++i)
	{
		if (ms_pools[i].IsActive()) continue;

		ms_pools[i].Activate(sBlockSize, sBlockSize, nNumInitialBlocks);
		index = i;

		return true;
	}

	// all occupied
	index = -1;
	return false;
}

//-------------------------------------------------------------------------------
void MemMgr::DeletePool(int nID)
{
	// will delete ALL pools with this id
	for (int i=0; i < MAXPOOLS; ++i)
	{
		if (ms_pools[i].IsActive() && (ms_pools[i].GetID()==nID))
		{
			DestroyChunk(&ms_pools[i]);
		}
	}
}

//-------------------------------------------------------------------------------
MemHeap * MemMgr::GetHeap()
{
	return &ms_heap;
}

//-------------------------------------------------------------------------------
MemPool * MemMgr::GetPool(UINT index)
{
	TASSERT( (index >= 0 && index < MAXPOOLS), "MemMgr -- Pool index out of range");
	return &ms_pools[index];
}

//-------------------------------------------------------------------------------
void MemMgr::DestroyChunk(IMemoryChunk * pChunk)
{
	pChunk->Deactivate();
}

//-------------------------------------------------------------------------------
void MemMgr::ShutDown()
{
	for (int i=0; i < MAXPOOLS; ++i)
		DestroyChunk(&ms_pools[i]);

	DestroyChunk(&ms_heap);
}

//-------------------------------------------------------------------------------
void MemMgr::PrintInfo (FILE *pFile)
{
	fprintf(pFile, "MemMgr::PrintInfo()\n");
	int totalbytes = 0;

	for (int i=0; i < MAXPOOLS; i++)
	{
		if (ms_pools[i].IsActive())
		{
			totalbytes += ms_pools[i].GetBytesAllocated();
			ms_pools[i].PrintInfo(pFile);
		}
	}

	ms_heap.PrintInfo(pFile);

	totalbytes += ms_heap.GetBytesAllocated();
	fprintf(pFile, "\nTotal Memory Used: %d bytes\n", totalbytes);
}

//-------------------------------------------------------------------------------
int MemMgr::ReportMemoryLeaks (FILE* pFile)
{
	int nLeaks = 0;

	// leaks in pools
	for (int i=0; i < MAXPOOLS; i++)
	{
		if (ms_pools[i].IsActive())
		{
			nLeaks += ms_pools[i].ReportMemoryLeaks(pFile);
		}
	}

	// leaks in heap
	if (ms_heap.IsActive())
	{
		nLeaks += ms_heap.ReportMemoryLeaks(pFile);
	}

	if (nLeaks > 0)
	{
		TDebugPrint(("%d memory leaks found", nLeaks));
		fprintf(pFile, "%d memory leaks found\n", nLeaks);
	}

	return nLeaks;
}

//-------------------------------------------------------------------------------
void MemMgr::PrintInfo(std::string & info)
{
	int pooledbytes = 0;
	int heapedbytes = 0;

	for (int i=0; i < MAXPOOLS; i++)
	{
		if (ms_pools[i].IsActive())
		{
			pooledbytes += ms_pools[i].GetBytesAllocated();
		}
	}

	heapedbytes += ms_heap.GetBytesAllocated();

	char buffer[64];
	sprintf_s(buffer, 64, "Memory Allocated: %d (pooled) %d (heaped)", pooledbytes, heapedbytes);
	info = buffer;
}
