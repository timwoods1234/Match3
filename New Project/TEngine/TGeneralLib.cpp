#include "TGeneralLib.h"
#include "TMemory.h"

//-------------------------------------------------------------------------------
void StringTokenize(const std::string& str,
			  std::vector<std::string>& tokens,
			  const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

//-------------------------------------------------------------------------------
template<class T>
bool RemoveFromVector(std::vector<T*> vec, T* element)
{
	for (unsigned int i=0; i<vec.size(); ++i)
	{
		if (vec[i] == element)
		{
			vec.erase(vec.begin() + i);
			return;
		}
	}
}

//-------------------------------------------------------------------------------
void GetAssetPath()
{
	// get the path of the exe
	char strExePath [MAX_PATH];
	GetModuleFileName (NULL, strExePath, MAX_PATH);
	std::string path = "";
	path.append(strExePath);

	// use this line if running outside the IDE
	path = path.substr( 0, path.rfind("\\") );

	// in VC++ folders are different (assets stored in "New Project" folder)
	if (IsDebuggerPresent())
	{
#if defined(DEBUG) | defined(_DEBUG)
	path = path.substr( 0, path.rfind("\\Debug") );
#else
	path = path.substr( 0, path.rfind("\\Release") );
#endif
	path.append("\\New Project");
	}

	// reset directory
	SetCurrentDirectory(path.c_str());
}