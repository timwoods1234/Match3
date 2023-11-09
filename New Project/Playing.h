#ifndef __PLAYING_H__
#define __PLAYING_H__

//+-------------------------------------------------------
//| The main game state, where the game is played
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class GridTile;
class Mover;

class Playing : public GameState, public TEventListener
{
public:
	Playing();
	virtual ~Playing();

	virtual void OnEntry();
	virtual void OnExit();
	virtual void OnUpdate(float elapsedTime);
	virtual void OnRender();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	void OnCombo();
	void Restart(const GuiItem* pItem);
	void EnableMovement();
	void DisableMovement();
	void Quit(const GuiItem* pItem);

protected:
	void Begin();
	void Finish();
	void UpdateUI();

	void OnDragBegin(const TVector2& position);
	void OnDragEnd(const TVector2& position);
	void UpdateEntityDragging(bool bReset);

	class GuiManager*	m_pGui;
	TAnimatedSprite		m_cowboySprite;
	GridTile*			m_pStartTile;
	GridTile*			m_pEndTile;
	float				m_fPlayTimer;
	float				m_fTimeLimit;
	bool				m_bInputEnabled;

	GridTile*			m_pDraggedTile;
	TVector3			m_dragStart;

	GuiItem*			m_pSelectedItemUI;
};

#endif
