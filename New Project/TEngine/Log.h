#ifndef __LOG_H__
#define __LOG_H__

//+-------------------------------------------------------
//| Singleton to print messages to file
//| Used for debugging
//| Version 1.002
//|
//| TODO: add separate debug log function (prints to debug_output and debug_log)
//| TODO: don't create the log file unless it's used.
//+-------------------------------------------------------

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

class Log : public Singleton<Log>
{
public:
	Log();
	~Log();

	void Print(const char *chString, ...);

	FILE * GetFile();

private:
	FILE *	m_pFile;
	bool	m_bActive;
};

DefineSingleton(Log);

#define TLog(x) Log::Get()->Print x

#endif
