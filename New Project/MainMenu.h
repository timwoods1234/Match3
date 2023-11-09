#ifndef __MAINMENU_H__
#define __MAINMENU_H__

//+-------------------------------------------------------
//| Title/tutorial screen
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class MainMenu : public GameState, public TEventListener
{
public:
	MainMenu();
	virtual ~MainMenu();

	virtual void OnEntry();
	virtual void OnExit();
	virtual void OnUpdate(float elapsedTime);
	virtual void OnRender();
	virtual void OnLostDevice() {}
	virtual void OnResetDevice();

	void Play(const GuiItem* pItem);
	void Quit(const GuiItem* pItem);

protected:
	class GuiManager* m_pGui;
	TSoundHandle m_MusicHandle;
};

#endif
