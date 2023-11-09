#include "Locator.h"
#include "MouseKeyInput.h"
#include "DirectAudio.h"
#include "D3DTextPrinter.h"
#include "TD3D.h"
#include "TETimer.h"
#include "Log.h"
#include "TTexture.h"
#include "TMesh.h"
#include "SafeRelease.h"
#include "TMemory.h"
#include "IRenderer.h"
#include "GameStateManager.h"
#include "TConsole.h"
#include "TError.h"
#include "Camera.h"
#include "TEventUpdate.h"
#include "TEventRender.h"

#include "TGame.h"
#include "EngineGlobals.h"

//-------------------------------------------------------------------------------
DefineSingleton(TGame);

bool TGame::bInitialized = false;

//-------------------------------------------------------------------------------
LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if (TGame::bInitialized)
	{
		return TGame::Get()->MsgProc(msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

//-------------------------------------------------------------------------------
void DrawGameStats()
{
	// Draw game stats
	Locator::GetTextPrinter()->DrawTextA(
		g_nDebugFont, 460, 10, 255, 255, 0, 
		"Frames Per Second: %.0f\n\
		Frame Speed (ms): %.2f\n\
		Triangles: %d\n\
		Vertices: %d\n\
		Vertex Memory: %dKB\n",
		g_fFramesPerSecond, 
		g_fMsPerFrame,
		Locator::GetRenderer()->GetNumTriangles(),
		Locator::GetRenderer()->GetNumVertices(),
		(TMesh::GetAllocatedMemory() / 1024) );
}

//-------------------------------------------------------------------------------
void TGame::OnDeviceLost()
{
	OnLostDevice();
	Locator::GetAudio()->OnLostDevice();
	Locator::GetTextPrinter()->OnLostDevice();
	Locator::GetRenderer()->OnLostDevice();

	Locator::GetRenderDevice()->ResetDevice();

	OnResetDevice();
	Locator::GetTextPrinter()->OnResetDevice();
	Locator::GetRenderer()->OnResetDevice();
}

//-------------------------------------------------------------------------------
LRESULT TGame::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Minimized or Maximized?
	static bool minOrMaxed = false;

	RECT clientRect = {0, 0, 0, 0};

	switch(msg)
	{
	// WM_ACTIVE (activated or deactivated)
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
			m_bPaused = true;
		else
			m_bPaused = false;
		return 0;

	// WM_SIZE (resize window)  
	case WM_SIZE:
		if( Locator::GetRenderDevice()->IsRunning() )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				m_bPaused	= true;
				minOrMaxed	= true;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				m_bPaused	= false;
				minOrMaxed	= true;

				OnDeviceLost();
			}

			// Restored (any resize that is not a minimize or maximize)
			else if( wParam == SIZE_RESTORED )
			{
				m_bPaused = false;

				OnDeviceLost();
			}
		}
		return 0;

	// WM_EXITSIZEMOVE (resize bars released)
	case WM_EXITSIZEMOVE:
		GetClientRect(g_hWindow, &clientRect);
		g_fWidth  = (float)clientRect.right;
		g_fHeight = (float)clientRect.bottom;
		Locator::GetRenderDevice()->SetScreenSize(clientRect.right, clientRect.bottom);

		OnDeviceLost();

		return 0;

	// WM_CLOSE 
	case WM_CLOSE:
		DestroyWindow(g_hWindow);
		return 0;

	// Window destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	} // switch

	return DefWindowProc(g_hWindow, msg, wParam, lParam);
}

//-------------------------------------------------------------------------------
TGame::TGame()
:	m_pTimer(0),
	m_bPaused(false),
	m_bDrawstats(false),
	m_bQuit(false),
	m_bWireframe(false),
	m_fDebugTimeMultiplier(1.f)
{
}

//-------------------------------------------------------------------------------
TGame::~TGame()
{
	if (bInitialized)
	{
		UnloadAssets();
		UnloadLibrary();
	}
}

//-------------------------------------------------------------------------------
void TGame::Initialize(HINSTANCE hInst, char* WndCaption)
{
	m_hInst		 = hInst;
	m_WndCaption = WndCaption;

	InitWindow();
	InitLibrary();

	// load white texture and billboard for all 2D
	TTexture::Load("Assets/3D/title.bmp");
	TMesh::Load("Assets/3D/textasset.x", &g_nGUI);
	LoadAssets();

	m_pTimer = TNew TETimer();
	m_pTimer->Init();
	m_pTimer->Start();

	Locator::GetTextPrinter()->CreateFont("Helvetica", FW_BOLD, false, 10, &g_nDebugFont);

	bInitialized = true;
}

//-------------------------------------------------------------------------------
void TGame::UnloadLibrary()
{
	TEventUpdate::Destroy();
	TEventRender::Destroy();

	TMesh::Unload();
	TTexture::Unload();

	IAudio * pAudio = Locator::GetAudio();
	TSafeDelete(pAudio);

	IInput * pInput = Locator::GetInput();
	TSafeDelete(pInput);

	ITextPrinter * pText = Locator::GetTextPrinter();
	TSafeDelete(pText);

	IRenderer * pRenderer = Locator::GetRenderer();
	TSafeDelete(pRenderer);

	IRenderDevice * pRenderDevice = Locator::GetRenderDevice();
	TSafeDelete(pRenderDevice);

	TConsole::Destroy();

	TSafeDelete(m_pTimer);
	TSafeDelete(g_pCamera);

	int nLeaks = MemMgr::ReportMemoryLeaks(Log::Get()->GetFile());
	TASSERT((nLeaks == 0), "Memory leaks were detected, see log file for details");
	MemMgr::ShutDown();

	Log::Destroy();
}

//-------------------------------------------------------------------------------
void TGame::InitWindow()
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "TGameWndClassName";

	if( !RegisterClass(&wc) )
	{
		TASSERT(false, "RegisterClass FAILED");
		PostQuitMessage(0);
		return;
	}

	// Create the window
	g_hWindow = CreateWindow("TGameWndClassName", m_WndCaption, 
		WS_OVERLAPPEDWINDOW, 0, 0, (int)g_fWidth, (int)g_fHeight, 
		0, 0, m_hInst, 0);

	if( !g_hWindow )
	{
		TASSERT(false, "CreateWindow FAILED");
		PostQuitMessage(0);
		return;
	}

	 SetWindowLong(g_hWindow, GWL_STYLE,
	               GetWindowLong(g_hWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);
	 SetWindowLong(g_hWindow, GWL_STYLE,
	               GetWindowLong(g_hWindow, GWL_STYLE) & ~WS_THICKFRAME);
	 SetWindowLong(g_hWindow, GWL_STYLE,
	               GetWindowLong(g_hWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX);

	ShowWindow(g_hWindow, SW_SHOWDEFAULT);
	UpdateWindow(g_hWindow);
}

//-------------------------------------------------------------------------------
void TGame::InitLibrary()
{
	MemMgr::Initialize();

	Log::Create();		// log file

	// 3d rendering
	TD3D * pTD3D = TNew TD3D();
	if (!(pTD3D->InitDevice(g_hWindow,
							(int)g_fWidth, (int)g_fHeight,
							0, g_bWindowed)))
	{
		TASSERT(false, "Failed to initialise graphics");
		PostQuitMessage(0);
		return;
	}
	Locator::RegisterRenderDevice(pTD3D);

	// mouse/keyboard input
	MouseKeyInput * pInput = TNew MouseKeyInput();
	if (!(pInput->Init(m_hInst, g_hWindow)))
	{
		TASSERT(false, "Failed to initialize input");
		PostQuitMessage(0);
		return;
	}
	Locator::RegisterInput(pInput);

	// sound
	DirectAudio * pDirectAudio = TNew DirectAudio();
	if (!(pDirectAudio->Init(g_hWindow)))
	{
		TASSERT(false, "Failed to initialize audio");
		PostQuitMessage(0);
		return;
	}
	Locator::RegisterAudio(pDirectAudio);

	// text printer
	D3DTextPrinter * pText = TNew D3DTextPrinter();
	Locator::RegisterTextPrinter(pText);

	// renderer & camera
	CreateRenderer();	// game-specific
	g_pCamera = TNew Camera();
	Locator::GetRenderer()->SetCurrentCamera(g_pCamera);

	TConsole::Create();
	TConsole::Get()->SetCallback(&TGame::ProcessConsoleCommand);

	TEventUpdate::Create();
	TEventRender::Create();
}

//-------------------------------------------------------------------------------
void TGame::ProcessInput()
{
	Locator::GetInput()->Update();
}

//-------------------------------------------------------------------------------
bool TGame::IsDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = Locator::GetRenderDevice()->GetDevice()->TestCooperativeLevel();

	if (hr == D3D_OK)
	{	
		// Early out if everything is ok
		return false;
	}

	if( hr == D3DERR_DEVICELOST )
	{	
		// Lost and can't be reset yet
		Sleep(20);
		return true;
	}

	else if( hr == D3DERR_DRIVERINTERNALERROR )
	{	
		// Driver error, exit.
		TASSERT(false, "Internal Driver Error ... Exiting");
		m_bQuit = true;
		return true;
	}

	else if( hr == D3DERR_DEVICENOTRESET )
	{	
		// The device is lost but can be reset and restored.
		OnDeviceLost();
		return false;
	}
	else
	{
		return false;
	}
}

//-------------------------------------------------------------------------------
int TGame::GameLoop()
{
    MSG msg; 
	msg.message = WM_NULL;

    while((msg.message != WM_QUIT) && (!m_bQuit))
    {
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{	
			// If there are Window messages then process them.
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}

		else
		{	
			// Otherwise Game runs
			if (m_bPaused)
			{
				Sleep(20);
				continue;
			}

			else if (!IsDeviceLost())
			{
				m_pTimer->Update();		// Update the timer
				m_pTimer->FrameMove();

				// Get fps info
				g_fElapsedTime		= m_pTimer->GetElapsedTime();
				g_fFramesPerSecond	= m_pTimer->GetFramesPerSecond();
				g_fMsPerFrame		= m_pTimer->GetMsPerFrame();

				if (g_fElapsedTime > 1.f)
				{	
					// ensure no undesired behaviour if the app idles between frames
					g_fElapsedTime = 0.01f;
				}
#if defined(DEBUG) | defined(_DEBUG)
				g_fElapsedTime *= m_fDebugTimeMultiplier;
#endif

				// Update Input
				ProcessInput();

				if (Locator::GetInput()->IsPressed(DIK_GRAVE))
				{	
					// toggle console with `
					TConsole::Get()->Toggle();
				}

				if (TConsole::Get()->IsActive())
				{	
					// either update the console
					TConsole::Get()->Update();
				}
				else
				{	
					// or update the game
					TEventUpdate::Get()->Execute(g_fElapsedTime);
				}

				if (Locator::GetRenderDevice()->IsRunning())
				{
					Locator::GetRenderDevice()->BeginRendering(true,true,false);

					if (TConsole::Get()->IsActive())
					{	
						// either draw the console
						TConsole::Get()->Print();
					}
					else
					{	
						// or draw the game
						Locator::GetRenderer()->SetNumTriangles(0);
						Locator::GetRenderer()->SetNumVertices(0);

						TEventRender::Get()->Execute();

						if (m_bDrawstats)
						{
							DrawGameStats();
						}
					}
	
					Locator::GetRenderDevice()->EndRendering();
				}	// RenderDevice IsRunning

			} // !IsDeviceLost()
		}
    } // while

	return (int)msg.wParam;
}

//-------------------------------------------------------------------------------
float TGame::GetAppTime() const
{
	return m_pTimer->GetAppTime();
}
