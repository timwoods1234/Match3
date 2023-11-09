#include "GameStateManager.h"
#include "TConsole.h"
#include "TGame.h"
#include "MemMgr.h"
#include "Locator.h"
#include "IRenderDevice.h"

//-------------------------------------------------------------------------------
// Generic console command responses (not app-specific)
// in their own .cpp so it doesn't clutter TGame.cpp

//-------------------------------------------------------------------------------
void TGame::ProcessConsoleCommand(TConsoleCmdParts & command)
{
	m_currentInstance->ConsoleCommand(command);
}

//-------------------------------------------------------------------------------
void TGame::AppConsoleCommand(TConsoleCmdParts & command)
{
	// one-word comms
	if		(command[0] == "quit")			{ PostQuitMessage(0); }
	else if (command[0] == "clear")			{ TConsole::Get()->Clear(); }
	else if (command[0] == "stats")			{ ToggleDrawstats(); }
	else if (command[0] == "memdump")		{ LogMemoryUsage(); }
	else if (command[0] == "memory")		{ PrintMemoryStats(); }
	else if (command[0] == "wireframe")		{ ToggleWireframe(); }

	// split comms
	else if (command[0] == "state")
	{
		if (command.size() == 1)
			PrintGameState();
		else if (command.size() > 1)
			SwitchState(command[1].c_str());
	}
	else if (command[0] == "speed")
	{
		if (command.size() == 1)
			PrintCurrentSpeed();
		else if (command.size() > 1)
			SetCurrentSpeed(command[1].c_str());
	}

	// print list of commands
	else if (command[0] == "help")
	{
		TCONSOLEF(("Available Commands:"));
		TCONSOLEF(("<-- quit -- clear -- stats -- speed -->"));
		TCONSOLEF(("<-- memdump -- memory  -- state -->"));
	}
}

//-------------------------------------------------------------------------------
void TGame::ToggleDrawstats()
{
	m_bDrawstats = !m_bDrawstats;

	if (m_bDrawstats)
	{
		TCONSOLEF( ("Drawstats ON") );
	}
	else
	{
		TCONSOLEF( ("Drawstats OFF") );
	}
}

//-------------------------------------------------------------------------------
void TGame::LogMemoryUsage()
{
	unsigned long time = timeGetTime();

	static char filename[128];
	sprintf_s(filename, 128, "memdump%ld.txt", time);

	FILE * file;
	fopen_s(&file, filename, "a+");
	MemMgr::PrintInfo(file);
	fclose(file);

	TCONSOLEF( ("Memory state saved to %s", filename) );
}

//-------------------------------------------------------------------------------
void TGame::PrintMemoryStats()
{
	std::string strInfo;
	MemMgr::PrintInfo(strInfo);
	TCONSOLEF((strInfo.c_str()));
}

//-------------------------------------------------------------------------------
void TGame::SwitchState(const char* state)
{
#if defined(DEBUG) | defined(_DEBUG)
	if (GameStateManager::Get()->SwitchState(state))
	{
		TCONSOLEF(("Swapping state to %s", state));
	}
	else
	{
		TCONSOLEF(("Can't find state '%s'", state));
	}
#else
	TCONSOLEF(("Debug only!"));
#endif
}

//-------------------------------------------------------------------------------
void TGame::PrintGameState()
{
	TCONSOLEF(("Current State: %s", GameStateManager::Get()->GetCurrentStateName()));
}

//-------------------------------------------------------------------------------
void TGame::ToggleWireframe()
{
	m_bWireframe = !m_bWireframe;

	if (m_bWireframe)
	{
		TCONSOLEF(("Wireframe ON"));
		Locator::GetRenderDevice()->SetShadeMode(RS_SHADE_TRIWIRE, 0.f);
	}
	else
	{
		TCONSOLEF(("Wireframe OFF"));
		Locator::GetRenderDevice()->SetShadeMode(RS_SHADE_SOLID, 0.f);
	}
}

//-------------------------------------------------------------------------------
void TGame::PrintCurrentSpeed()
{
	TCONSOLEF(("Current Speed %.2f", m_fDebugTimeMultiplier));
}

//-------------------------------------------------------------------------------
void TGame::SetCurrentSpeed(const char* speed)
{
#if defined(DEBUG) | defined(_DEBUG)
	float fNewSpeed = (float)atof(speed);
	m_fDebugTimeMultiplier = fNewSpeed;
#else
	TCONSOLEF(("Debug only!"));
#endif
}
