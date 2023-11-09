#include "EngineGlobals.h"

//-------------------------------------------------------------------------------
HWND	g_hWindow			= NULL;
UINT	g_nDebugFont		= 0;
UINT	g_nGameFont			= 0;	// use debug font unless the game creates its own font
UINT	g_nGUI				= 0;

bool	g_bWindowed			= true;
float	g_fWidth			= 640.0f;
float	g_fHeight			= 480.0f;

float	g_fFramesPerSecond	= 0.0f;
float	g_fElapsedTime		= 0.0f;
float	g_fMsPerFrame		= 0.0f;

Camera*	g_pCamera			= NULL;

TMatrix	g_matWorld;
TMatrix	g_matScale;
TMatrix	g_matRotate;
TMatrix	g_matTranslate;