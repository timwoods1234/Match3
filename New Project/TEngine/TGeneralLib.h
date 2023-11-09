#ifndef __TGENERALLIB_H__
#define __TGENERALLIB_H__

//+-------------------------------------------------------
//| File TGeneralLib.h
//| general helper functions
//| Version 1.007
//+-------------------------------------------------------

#include <windows.h>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------
#define FOREACH(list, name) for (unsigned int name=0; name<list.size(); ++name)

//-------------------------------------------------------------------------------
DWORD	FtoDw(float f);

//-------------------------------------------------------------------------------
// Split a std::string around a token
void	StringTokenize(const std::string& str,
			  std::vector<std::string>& tokens,
			  const std::string& delimiters = " ");

//-------------------------------------------------------------------------------
// Remove first occurance of <element> from <vec>
template<class T>
bool RemoveFromVector(std::vector<T*> vec, T* element);

//-------------------------------------------------------------------------------
// Reset the application path to the directory that contains the .exe
void GetAssetPath();

#endif
