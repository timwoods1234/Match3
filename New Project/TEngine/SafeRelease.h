#ifndef __SAFERELEASE_H__
#define __SAFERELEASE_H__

//-------------------------------------------------------
// SafeRelease.h
// Holds cleanup functions.
// Version 1.005
//-------------------------------------------------------

#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

//------------------------------------------------------------------------
/* --- Safe deletion --- */
//------------------------------------------------------------------------
#define SafeRelease(pInterface) if(pInterface != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject) if(pObject != NULL) {delete pObject; pObject=NULL;}
#define SafeDeleteArray(p) { if(p) { delete[] (p);   (p)=NULL; } }

#endif