#include "MainMenu.h"
#include "TEventUpdate.h"
#include "TEventRender.h"

#include "Globals.h"

//-------------------------------------------------------------------------------
MainMenu::MainMenu()
{
	m_strStateName = "MAINMENU";

	m_pGui = TNew GuiManager();
	m_pGui->LoadFile("Assets/Data/UI/Menu.txt");

	m_pGui->RegisterCallback("playButton", Gui_Release, this, &MainMenu::Play);
	m_pGui->RegisterCallback("quitButton", Gui_Release, this, &MainMenu::Quit);

	Locator::GetAudio()->LoadSound("Assets/Sound/music.wav", &m_MusicHandle, true);
}

//-------------------------------------------------------------------------------
MainMenu::~MainMenu()
{
	TSafeDelete(m_pGui);
}

//-------------------------------------------------------------------------------
void MainMenu::OnEntry()
{
	Locator::GetAudio()->PlaySound(m_MusicHandle);

	TEventUpdate::Get()->Register(this, &MainMenu::OnUpdate);
	TEventRender::Get()->Register(this, &MainMenu::OnRender);
	OnResetDevice();
}

//-------------------------------------------------------------------------------
void MainMenu::OnExit()
{
	TEventUpdate::Get()->Unregister(this, &MainMenu::OnUpdate);
	TEventRender::Get()->Unregister(this, &MainMenu::OnRender);
}

//-------------------------------------------------------------------------------
void MainMenu::OnResetDevice()
{
	Locator::GetRenderer()->Camera2D();
	Locator::GetRenderDevice()->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	Locator::GetRenderDevice()->GetDevice()->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS);
}

//-------------------------------------------------------------------------------
void MainMenu::OnUpdate(float elapsedTime)
{
	g_pCursor->Update();

	m_pGui->Update(*g_pCursor);
}

//-------------------------------------------------------------------------------
void MainMenu::OnRender()
{
	Locator::GetRenderer()->Begin(0);
	Locator::GetRenderer()->BeginPass(0, 0);

	m_pGui->Render();
	g_pCursor->Render();

	Locator::GetRenderer()->EndPass(0);
	Locator::GetRenderer()->End(0);
}

//-------------------------------------------------------------------------------
void MainMenu::Play(const GuiItem* pItem)
{
	GameStateManager::Get()->SwitchState("PLAYING");
}

//-------------------------------------------------------------------------------
void MainMenu::Quit(const GuiItem* pItem)
{
	PostQuitMessage(0);
}
