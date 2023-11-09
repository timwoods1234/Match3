#ifndef __TGAME_H__
#define __TGAME_H__

//+----------------------------------------
//| Game class.
//| Rather than extend, apps will implement LoadAssets
//| and use game states for their own behaviour.
//| Version 1.000
//+----------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef __TCONSOLE_H__
#include "TConsole.h"
#endif

class TGame : public Singleton<TGame>
{
public:
	static bool	bInitialized;

	// callback for TConsole commands
	// passes command to the game instance
	static void ProcessConsoleCommand(TConsoleCmdParts & command);

				TGame();
	virtual		~TGame();

	void		Initialize(HINSTANCE hInst, char* WndCaption);
	int			GameLoop();
	LRESULT		MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	// the total time the application has been running
	float		GetAppTime() const;

protected:
	class TETimer* m_pTimer;

	HINSTANCE	m_hInst;
	char*		m_WndCaption;
	bool		m_bDrawstats;
	bool		m_bPaused;
	bool		m_bQuit;
	bool		m_bWireframe;
	float		m_fDebugTimeMultiplier;

	void		InitLibrary();
	void		InitWindow();
	void		UnloadLibrary();
	bool		IsDeviceLost();
	void		ProcessInput();
	void		OnDeviceLost();

	// console commands
	void		AppConsoleCommand(TConsoleCmdParts & command);
	void		ToggleDrawstats();
	void		LogMemoryUsage();
	void		PrintMemoryStats();
	void		SwitchState(const char* state);
	void		PrintGameState();
	void		ToggleWireframe();
	void		PrintCurrentSpeed();
	void		SetCurrentSpeed(const char* speed);

	// game-specific
	void		ConsoleCommand(TConsoleCmdParts & command);
	void		CreateRenderer();
	void		LoadAssets();
	void		UnloadAssets();
	void		OnLostDevice();
	void		OnResetDevice();
};

#endif
