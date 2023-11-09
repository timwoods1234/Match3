#include "TEngine.h"
#include "TEventUpdate.h"
#include "TEventRender.h"
#include "TEventRender2D.h"

#include "Playing.h"
#include "Grid.h"
#include "Globals.h"
#include "GridTile.h"
#include "EventMoveFinished.h"
#include "EventMoveBegun.h"
#include "Player.h"
#include "SpritePlugin.h"
#include "EventCombo.h"

//-------------------------------------------------------------------------------
static const TString scoreStr = "scoreValue";
static const TString timerStr = "timerValue";
static const TString comboAnimStr = "Idle";

//-------------------------------------------------------------------------------
Playing::Playing()
:	m_pStartTile(NULL),
	m_pEndTile(NULL),
	m_pDraggedTile(NULL),
	m_bInputEnabled(false),
	m_fTimeLimit(55.f)
{
	DataLoader settings;
	settings.LoadFile("Assets/Data/Settings.txt");
	settings.GetFloat("TimeLimit", m_fTimeLimit, true);

	m_strStateName = "PLAYING";

	m_pGui = TNew GuiManager();
	m_pGui->LoadFile("Assets/Data/UI/Playing.txt");

	m_pGui->RegisterCallback("restartButton", Gui_Release, this, &Playing::Restart);
	m_pGui->RegisterCallback("quitButton", Gui_Release, this, &Playing::Quit);
	m_pSelectedItemUI = m_pGui->GetButtonByName("selection");

	m_cowboySprite.Load("Assets/2D/Cowboy/Cowboy.txt");
}

//-------------------------------------------------------------------------------
Playing::~Playing()
{
	TSafeDelete(m_pGui);
}

//-------------------------------------------------------------------------------
void Playing::OnResetDevice()
{
}

//-------------------------------------------------------------------------------
void Playing::OnLostDevice()
{
}

//-------------------------------------------------------------------------------
void Playing::OnEntry()
{
	TEventUpdate::Get()->Register(this, &Playing::OnUpdate);
	TEventRender::Get()->Register(this, &Playing::OnRender);
	EventMoveBegun::Get()->Register(this, &Playing::DisableMovement);
	EventMoveFinished::Get()->Register(this, &Playing::EnableMovement);
	EventCombo::Get()->Register(this, &Playing::OnCombo);

	Begin();
}

//-------------------------------------------------------------------------------
void Playing::OnExit()
{
	TEventUpdate::Get()->Unregister(this, &Playing::OnUpdate);
	TEventRender::Get()->Unregister(this, &Playing::OnRender);
	EventMoveBegun::Get()->Unregister(this, &Playing::DisableMovement);
	EventMoveFinished::Get()->Unregister(this, &Playing::EnableMovement);
	EventCombo::Get()->Unregister(this, &Playing::OnCombo);
}

//-------------------------------------------------------------------------------
void Playing::OnUpdate(float elapsedTime)
{
	g_pCursor->Update();
	m_pGui->Update(*g_pCursor);

	m_cowboySprite.Update(elapsedTime);

	if (m_bInputEnabled)
	{
		if (g_pCursor->GetState() == Cursor_Released)
		{
			OnDragEnd(g_pCursor->GetPosition());
		}
		else if (g_pCursor->GetState() == Cursor_Pressed)
		{
			OnDragBegin(g_pCursor->GetPosition());
		}
	}

	m_fPlayTimer -= elapsedTime;

	if (m_fPlayTimer >= 0.f)
	{
		UpdateUI();
	}
	else
	{
		Finish();
	}
}

//-------------------------------------------------------------------------------
void Playing::UpdateUI()
{
	int minutes = (int)m_fPlayTimer/60;
	int seconds = (int)(m_fPlayTimer)%60;

	m_pGui->SetValue(timerStr, ("%d:%.2d"), minutes, seconds);

	int score = Player::Get()->GetScore();

	m_pGui->SetValue(scoreStr, ("%d"), score);

	if (m_pSelectedItemUI != NULL)
	{
		if (m_pStartTile != NULL)
		{
			m_pSelectedItemUI->SetVisible(true);

			TVector3 position = m_pStartTile->GetEntity()->GetPosition();

			m_pSelectedItemUI->SetPosition( TVector2(position.x, position.y) );
		}
		else
		{
			m_pSelectedItemUI->SetVisible(false);
		}
	}

	UpdateEntityDragging(false);
}

//-------------------------------------------------------------------------------
void Playing::UpdateEntityDragging(bool bReset)
{
	if (m_pDraggedTile != NULL)
	{
		Entity* pEntityDragging = m_pDraggedTile->GetEntity();

		if (bReset)
		{
			pEntityDragging->SetPosition(m_dragStart);
		}
		else
		{
			TVector2 cursorPosition = g_pCursor->GetPosition();

			cursorPosition -= TVector2(
								(float)(Grid::Get()->GetTileSize()/2), 
								(float)(Grid::Get()->GetTileSize()/2));

			pEntityDragging->SetPosition( TVector3(cursorPosition.x, cursorPosition.y, 0.0f) );
		}
	}
}

//-------------------------------------------------------------------------------
void Playing::OnRender()
{
	Locator::GetRenderer()->Begin(0);
	Locator::GetRenderer()->BeginPass(0, 0);

	Locator::GetRenderDevice()->SetClearColor(0,0,0);

	m_pGui->Render();
	m_cowboySprite.Render( TVector2(64,256) );

	TEventRender2D::Get()->Execute();

	g_pCursor->Render();

	Locator::GetRenderer()->EndPass(0);
	Locator::GetRenderer()->End(0);
}

//-------------------------------------------------------------------------------
void Playing::Begin()
{
	g_pCursor->SetState(Cursor_Up);

	m_fPlayTimer = m_fTimeLimit;

	m_pGui->SetVisible("restartButton", false);
	m_pSelectedItemUI->SetVisible(false);

	m_pStartTile = NULL;
	m_pEndTile = NULL;

	m_bInputEnabled = true;
}

//-------------------------------------------------------------------------------
void Playing::Finish()
{
	GameStateManager::Get()->SwitchState("GAMEOVER");
}

//-------------------------------------------------------------------------------
void Playing::Restart(const GuiItem* pItem)
{
	Begin();
}

//-------------------------------------------------------------------------------
void Playing::EnableMovement()
{
	m_bInputEnabled = true;
}

//-------------------------------------------------------------------------------
void Playing::DisableMovement()
{
	m_bInputEnabled = false;
}

//-------------------------------------------------------------------------------
void Playing::Quit(const GuiItem* pItem)
{
	PostQuitMessage(0);
}

//-------------------------------------------------------------------------------
void Playing::OnDragBegin(const TVector2& position)
{
	GridTile* pTile = Grid::Get()->GetTile(position);

	if (pTile != NULL)
	{
		m_pDraggedTile = pTile;

		m_dragStart = pTile->GetEntity()->GetPosition();
	}
}

//-------------------------------------------------------------------------------
void Playing::OnDragEnd(const TVector2& position)
{
	UpdateEntityDragging(true);
	GridTile* pTile = Grid::Get()->GetTile(position);

	// on click (press & release on same tile)
	if (pTile != NULL && pTile == m_pDraggedTile)
	{
		if (m_pStartTile == NULL)
		{
			m_pStartTile = pTile;
		}
		else
		{
			if (pTile != m_pStartTile)
			{
				m_pEndTile = pTile;
			}
			else
			{
				m_pStartTile = NULL;
			}
		}

		if (m_pStartTile != NULL && m_pEndTile != NULL)
		{
			Grid::Get()->SwapTiles(m_pStartTile, m_pEndTile);

			m_pStartTile = NULL;
			m_pEndTile = NULL;
		}
	}
	else
	{
		// dragged one tile onto another
		if (m_pDraggedTile != NULL && pTile != NULL)
		{
			Grid::Get()->SwapTiles(m_pDraggedTile, pTile);

			m_pStartTile = NULL;
			m_pEndTile = NULL;
		}
	}

	m_pDraggedTile = NULL;
}

//-------------------------------------------------------------------------------
void Playing::OnCombo()
{
	if (comboAnimStr != m_cowboySprite.GetCurrentAnimation())
	{
		m_cowboySprite.SetAnimation("Idle");
	}
}
