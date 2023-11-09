#include <stdio.h>

#include "Log.h"

//-------------------------------------------------------------------------------
Log::Log()
:	m_bActive(false)
{

}

//-------------------------------------------------------------------------------
Log::~Log()
{
	// close log
	if (m_bActive)
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}

		m_bActive = false;
	}
}

//-------------------------------------------------------------------------------
void Log::Print(const char *chString, ...)
{
	if (!m_bActive)
	{
		fopen_s(&m_pFile, "log.txt", "a+");	// Open file
		m_bActive = true;
	}

	static char	buffer[1024];
	char		*pArgs;

	pArgs = (char*) &chString + sizeof(chString);
	vsprintf_s(buffer, 1024, chString, pArgs);

	fprintf(m_pFile, buffer);
	fprintf(m_pFile, "\n");

	fflush(m_pFile);
}

//-------------------------------------------------------------------------------
FILE * Log::GetFile()
{
	if (!m_bActive)
	{
		fopen_s(&m_pFile, "log.txt", "a+");	// Open file
		m_bActive = true;
	}

	return m_pFile;
}