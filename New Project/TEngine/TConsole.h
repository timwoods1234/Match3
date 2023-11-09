#ifndef __TCONSOLE_H__
#define __TCONSOLE_H__ 

//+-------------------------------------------------------
//| In-game console
//| Version 1.000
//+-------------------------------------------------------

#include <vector>
#include <string>

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef __CONSOLECALLBACK_DECLARED__
#define __CONSOLECALLBACK_DECLARED__
	typedef std::vector<std::string> TConsoleCmdParts;
	typedef void (*ConsoleCallback)(TConsoleCmdParts &);
#endif

class TConsole : public Singleton<TConsole>
{
public:
	TConsole();
	~TConsole();

	void SetCallback(ConsoleCallback pCallbackFunc);
	void Enable(bool bEnable);
	void Update();		// poll input and update the text
	bool IsActive();
	void SetTextColour(unsigned char r, unsigned char g, unsigned char b);
	void Toggle();
	void Print();
	void Clear();
	void PushString(const char* chString, ...);

protected:
	void Start();
	void Stop();
	void Execute();
	void AddNewCommand();

	std::string & GetCurrentCommand();
	std::string PreviousCommand;

	bool m_bActive;
	TConsoleCmdParts m_text;

	ConsoleCallback m_pCallbackFunc;

	unsigned char	m_r, m_g, m_b;
	unsigned int	m_nFontID;
	unsigned int	m_nFontSize;
	unsigned int	m_nMaxStrings;
};

#define TCONSOLEF(x)	TConsole::Get()->PushString x

#endif
