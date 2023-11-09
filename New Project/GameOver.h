#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

//+-------------------------------------------------------
//| In this state the game has finished
//| and can be continued or restarted
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class GameOver : public GameState, public TEventListener
{
public:
	GameOver();
	virtual ~GameOver();

	virtual void OnEntry();
	virtual void OnExit();
	virtual void OnUpdate(float elapsedTime);
	virtual void OnRender();
	virtual void OnLostDevice() {}
	virtual void OnResetDevice();

	void Restart(const GuiItem* pItem);
	void Continue(const GuiItem* pItem);
	void Quit(const GuiItem* pItem);

protected:
	class GuiManager* m_pGui;
};

#endif
