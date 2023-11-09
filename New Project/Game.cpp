#include "TEngine.h"
#include "D3DRenderer.h"
#include "Globals.h"
#include "TEventRender2D.h"

#include "Playing.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "EventMoveBegun.h"
#include "EventMoveFinished.h"
#include "EventCombo.h"
#include "MoverManager.h"
#include "SpriteAssigner.h"
#include "Player.h"
#include "Grid.h"

//-------------------------------------------------------------------------------
void TGame::LoadAssets()
{
	Locator::GetTextPrinter()->CreateFont("Arial", FW_BOLD, false, 24, &g_nGameFont);

	// Globals
	TEventRender2D::Create();
	EventMoveBegun::Create();
	EventMoveFinished::Create();
	EventCombo::Create();
	MoverManager::Create();
	SpriteAssigner::Create();
	Player::Create();
	Grid::Create();

	g_pCursor = TNew GuiCursor();
	g_pCursor->LoadTexture("Assets/2D/cursor.png");
	g_pCursor->SetBounds(0,0,g_fWidth, g_fHeight);

	TEffectManager::Create();
	TEffectManager::Get()->PreloadFromFile("Assets/Data/Effects.txt");

	// Load settings
	DataLoader settings;
	settings.LoadFile("Assets/Data/Settings.txt");

	float volume = 0.6f;
	settings.GetFloat("volume", volume, true);
	Locator::GetAudio()->SetVolume(volume);

	int continueCost = 1;
	settings.GetInt("continueCost", continueCost, true);
	Player::Get()->SetContinueCost(continueCost);

	// Create Game States
	GameStateManager::Create();
	GameStateManager::Get()->AddState(TNew MainMenu);
	GameStateManager::Get()->AddState(TNew GameOver);
	GameStateManager::Get()->AddState(TNew Playing);

	OnResetDevice();
	GameStateManager::Get()->SwitchState("MAINMENU");
}

//-------------------------------------------------------------------------------
void TGame::UnloadAssets()
{
	TSafeDelete(g_pCursor);

	Grid::Destroy();
	Player::Destroy();
	SpriteAssigner::Destroy();
	MoverManager::Destroy();
	GameStateManager::Destroy();
	TEffectManager::Destroy();
	TEventRender2D::Destroy();
	EventMoveBegun::Destroy();
	EventMoveFinished::Destroy();
	EventCombo::Destroy();
}

//-------------------------------------------------------------------------------
void TGame::OnResetDevice()
{
	// Initialize camera
	g_pCamera->SetViewDistance(5000.0f);
	g_pCamera->SetScreenSize(g_fWidth, g_fHeight);

	// App-specific render states
	Locator::GetRenderDevice()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Locator::GetRenderDevice()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Locator::GetRenderDevice()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GameStateManager::Get()->OnResetDevice();
}

//-------------------------------------------------------------------------------
void TGame::OnLostDevice()
{
	GameStateManager::Get()->OnLostDevice();
}

//-------------------------------------------------------------------------------
void TGame::ConsoleCommand(TConsoleCmdParts & command)
{
	AppConsoleCommand(command);
}

//-------------------------------------------------------------------------------
void TGame::CreateRenderer()
{
	// create game-specific renderer here
	// in this case, just the generic d3d renderer
	D3DRenderer * pRenderer = TNew D3DRenderer();
	Locator::RegisterRenderer(pRenderer);
}