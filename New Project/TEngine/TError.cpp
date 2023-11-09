#include <stdio.h>
#include "TError.h"

//-------------------------------------------------------------------------------
void _TAssert(const char* condition, const char* message, const char* file, const char* function, int line)
{
	// assert box
	static char buffer [512];
	sprintf_s(&buffer[0], 512, "%s\n%s\nFile:%s\nFunction:%s\nLine:%d\n",
		message, condition, file, function, line);
	MessageBox(0, &buffer[0], "Assertion Failed", MB_OK | MB_ICONSTOP);

	// print to debug output too
	OutputDebugString(buffer);

	// close the program in release mode
#if defined(DEBUG) | defined(_DEBUG)
	return;
#else
	_flushall();
	FatalAppExit(0,buffer);
#endif
}

//-------------------------------------------------------------------------------
void _TDebugPrint(const char* chString, ...)
{
	static char	buffer[1024];
	char		*pArgs;

	pArgs = (char*) &chString + sizeof(chString);
	vsprintf_s(buffer, 1024, chString, pArgs);

	OutputDebugString(buffer);
	OutputDebugString("\n");
}
