#ifndef __ENGINEGLOBALS_H__
#define __ENGINEGLOBALS_H__

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef __TMATRIX_H__
#include "TMatrix.h"
#endif

extern HWND				g_hWindow;
extern UINT				g_nDebugFont;
extern UINT				g_nGameFont;
extern bool				g_bWindowed;
extern UINT				g_nGUI;

extern float			g_fWidth;
extern float			g_fHeight;
extern float			g_fFramesPerSecond;
extern float			g_fElapsedTime;
extern float			g_fMsPerFrame;

extern class Camera*	g_pCamera;
extern TMatrix			g_matWorld;
extern TMatrix			g_matScale; 
extern TMatrix			g_matRotate;
extern TMatrix			g_matTranslate;

#endif