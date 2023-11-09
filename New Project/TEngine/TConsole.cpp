#include "TConsole.h"
#include "MouseKeyInput.h"
#include "Locator.h"
#include "ITextPrinter.h"
#include "TGeneralLib.h"

//-------------------------------------------------------------------------------
DefineSingleton(TConsole);

//-------------------------------------------------------------------------------
TConsole::TConsole()
:	m_bActive(false),
	m_r(255),
	m_g(255),
	m_b(255),
	m_nFontSize(12),
	m_nMaxStrings(30)
{
	Clear();

	Locator::GetTextPrinter()->CreateFont("Helvetica", FW_BOLD, false, m_nFontSize, &m_nFontID);

	PreviousCommand = "";

	PushString("TConsole v%.2f", 1.00f);
}

//-------------------------------------------------------------------------------
TConsole::~TConsole()
{
	m_text.clear();
}

//-------------------------------------------------------------------------------
void TConsole::SetCallback(ConsoleCallback pCallbackFunc)
{
	m_pCallbackFunc = pCallbackFunc;
}

//-------------------------------------------------------------------------------
void TConsole::Enable(bool bEnable)
{
	m_bActive = bEnable;
}

//-------------------------------------------------------------------------------
void TConsole::Update()
{
	if (Locator::GetInput()->IsPressed(DIK_RETURN))
	{
		Execute();
		return;
	}

	if (Locator::GetInput()->IsAnyKeyClicked() == false)
	{
		return;
	}

	if (Locator::GetInput()->IsPressed(DIK_UP))
	{
		GetCurrentCommand() = PreviousCommand;
		return;
	}

	// poll input and update the text
	char c = Locator::GetInput()->GetClickedChar();

	// backspace (8) clears last char
	if (c == 8)
	{
		if (GetCurrentCommand().size() > 0)
		{
			GetCurrentCommand().erase(GetCurrentCommand().begin() + GetCurrentCommand().size() - 1);
		}
	}
	else
	{
		static char buffer[8];
		sprintf_s(buffer, 8, "%c", c);
		GetCurrentCommand().append(buffer);
	}
}

//-------------------------------------------------------------------------------
bool TConsole::IsActive()
{
	return m_bActive;
}

//-------------------------------------------------------------------------------
void TConsole::Start()
{
	Enable(true);
}

//-------------------------------------------------------------------------------
void TConsole::Stop()
{
	Enable(false);
}

//-------------------------------------------------------------------------------
void TConsole::Print()
{
	static int frame = 0;
	frame++;
	if (frame > 24) { frame = -24; }

	static unsigned int nSpacing = m_nFontSize + (m_nFontSize/6);

	for (unsigned int i=0; i<m_text.size() - 1; ++i)
	{
		Locator::GetTextPrinter()->DrawText(m_nFontID, 0, i*nSpacing, m_r, m_g, m_b, m_text[i].c_str());
	}

	// last command highlighted
	Locator::GetTextPrinter()->DrawText(
		m_nFontID, 0, m_text.size()*nSpacing+2, m_r, m_g, m_b, ">");

	Locator::GetTextPrinter()->DrawText(
		m_nFontID, 12, m_text.size()*nSpacing+2, m_r, m_g, m_b, GetCurrentCommand().c_str());

	if (frame < 0)
	{
		Locator::GetTextPrinter()->DrawText(
			m_nFontID, 16, m_text.size()*nSpacing+2, m_r, m_g, m_b, "_");
	}
}

//-------------------------------------------------------------------------------
void TConsole::Clear()
{
	for (unsigned int i=0; i<m_text.size(); ++i)
	{
		m_text[i].clear();
	}
	m_text.clear();
}

//-------------------------------------------------------------------------------
void TConsole::Execute()
{
	if (m_text.size() > 0)
	{
		if (GetCurrentCommand() == "")
		{
			return;
		}

		PreviousCommand = GetCurrentCommand();
		AddNewCommand();

		TConsoleCmdParts Command;
		StringTokenize(PreviousCommand, Command);

		m_pCallbackFunc(Command);
	}
}

//-------------------------------------------------------------------------------
void TConsole::SetTextColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

//-------------------------------------------------------------------------------
std::string & TConsole::GetCurrentCommand()
{
	if (m_text.size() == 0)
	{
		AddNewCommand();
	}

	return m_text[ m_text.size() - 1 ];
}

//-------------------------------------------------------------------------------
void TConsole::AddNewCommand()
{
	std::string strNewCommand = "";
	m_text.push_back(strNewCommand);

	if (m_text.size() >= m_nMaxStrings)
	{
		m_text.erase( m_text.begin() );
	}
}

//-------------------------------------------------------------------------------
void TConsole::Toggle()
{
	if (IsActive())
	{
		Stop();
	}
	else
	{
		Start();
	}
}

//-------------------------------------------------------------------------------
void TConsole::PushString(const char* chString, ...)
{
	static char	buffer[256];
	char		*pArgs;

	pArgs = (char*) &chString + sizeof(chString);
	vsprintf_s(buffer, 256, chString, pArgs);

	GetCurrentCommand() = buffer;
	AddNewCommand();
}
