#ifndef __TERROR_H__
#define __TERROR_H__

//----------------------------------
// TError.h
// Holds debug and error functions.
// Version 1.001
//----------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef _DXERR_H_
#include <dxerr.h>
#endif

//-------------------------------------------------------------------------------
void _TAssert(const char* condition, const char* message, const char* file, const char* function, int line);
void _TDebugPrint(const char *chString, ...);

//-------------------------------------------------------------------------------
// Assert macro, for debug builds only
#if defined(DEBUG) | defined(_DEBUG)
#	define TASSERT(condition, message)	if ((!condition)) _TAssert(#condition, message, __FILE__, __FUNCTION__, __LINE__)
#else
#	pragma warning (disable : 4390)		// empty control statement, intended
#	define TASSERT(x, y)
#endif

//-------------------------------------------------------------------------------
// Debug print macro, for debug builds only
#if defined(DEBUG) | defined(_DEBUG)
#	define TDebugPrint(x)		  _TDebugPrint x
#else
#	define TDebugPrint(x)
#endif

//-------------------------------------------------------------------------------
// Debugging trace method for functions that throw HRESULT (uses dxerror)
#if defined(DEBUG) | defined(_DEBUG)
#	ifndef HR
#	define HR(x)										\
	   {                                                \
	   HRESULT hr = x;                                  \
	   if(FAILED(hr))                                   \
			 {                                          \
			 DXTrace(__FILE__, __LINE__, hr, #x, TRUE); \
			 }                                          \
	   }
#	endif
#	else
#	ifndef HR
#	define HR(x) x;
#	endif
#endif

#endif
