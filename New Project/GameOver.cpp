#include "GameOver.h"
#include "TEventUpdate.h"
#include "TEventRender.h"

#include "Grid.h"
#include "Globals.h"
#include "Player.h"

//-------------------------------------------------------------------------------
GameOver::GameOver()
{
	m_strStateName = "GAMEOVER";

	m_pGui = TNew GuiManager();
	m_pGui->LoadFile("Assets/Data/UI/GameOver.txt");

	m_pGui->RegisterCallback("continueButton", Gui_Release, this, &GameOver::Continue);
	m_pGui->RegisterCallback("restartButton", Gui_Release, this, &GameOver::Restart);
	m_pGui->RegisterCallback("quitButton", Gui_Release, this, &GameOver::Quit);
}

//-------------------------------------------------------------------------------
GameOver::~GameOver()
{
	TSafeDelete(m_pGui);
}

//-------------------------------------------------------------------------------
void GameOver::OnEntry()
{
	TEventUpdate::Get()->Register(this, &GameOver::OnUpdate);
	TEventRender::Get()->Register(this, &GameOver::OnRender);
	OnResetDevice();

	if (Player::Get()->GetScore() > Player::Get()->GetContinueCost())
	{
		m_pGui->SetVisible("continueButton", true);
		m_pGui->SetVisible("continueValue", true);
	}
	else
	{
		m_pGui->SetVisible("continueButton", false);
		m_pGui->SetVisible("continueValue", false);
	}

	m_pGui->SetValue("continueValue", ("%d"), Player::Get()->GetContinueCost());
}

//-------------------------------------------------------------------------------
void GameOver::OnExit()
{
	TEventUpdate::Get()->Unregister(this, &GameOver::OnUpdate);
	TEventRender::Get()->Unregister(this, &GameOver::OnRender);
}

//-------------------------------------------------------------------------------
void GameOver::OnResetDevice()
{
	Locator::GetRenderer()->Camera2D();
	Locator::GetRenderDevice()->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	Locator::GetRenderDevice()->GetDevice()->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS);
}

//-------------------------------------------------------------------------------
void GameOver::OnUpdate(float elapsedTime)
{
	g_pCursor->Update();

	m_pGui->Update(*g_pCursor);
}

//-------------------------------------------------------------------------------
void GameOver::OnRender()
{
	Locator::GetRenderer()->Begin(0);
	Locator::GetRenderer()->BeginPass(0, 0);

	m_pGui->Render();
	g_pCursor->Render();

	Locator::GetRenderer()->EndPass(0);
	Locator::GetRenderer()->End(0);
}

//-------------------------------------------------------------------------------
void GameOver::Restart(const GuiItem* pItem)
{
	Player::Get()->Reset();

	Grid::Get()->Reset();

	GameStateManager::Get()->SwitchState("PLAYING");
}

//-------------------------------------------------------------------------------
void GameOver::Continue(const GuiItem* pItem)
{
	Player::Get()->AddScore( -1 * Player::Get()->GetContinueCost() );

	GameStateManager::Get()->SwitchState("PLAYING");
}

//-------------------------------------------------------------------------------
void GameOver::Quit(const GuiItem* pItem)
{
	PostQuitMessage(0);
}
