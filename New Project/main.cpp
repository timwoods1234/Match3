#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "TEngine.lib")

#include "TEngine.h"

//+----------------------------------------------
//| Memory Management
//+----------------------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#	include <memory>
#endif

//-------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	// enable windowed mode from command line
	if (strcmp("windowed", cmdLine) ==0)
	{
		g_bWindowed = true;
	}

	g_fWidth = 755;
	g_fHeight = 600;

	TGame::Create();
	TGame::Get()->Initialize(hInstance, "Game Window");

	if (TGame::bInitialized)
	{
		TGame::Get()->GameLoop();
		TGame::Destroy();
	}

 	return 0;
}
