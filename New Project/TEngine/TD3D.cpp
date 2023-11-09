#include "TD3D.h"
#include "SafeRelease.h"
#include "TError.h"
#include <d3dx9.h>

//-------------------------------------------------------------------------------
TD3D::TD3D()
{
	m_pD3D					= NULL;
	m_pDevice				= NULL;
	m_pClearColor			= D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);

	m_bIsSceneRunning		= false;
	m_bRunning				= false;
	m_bStencil				= false;
	m_g_bWindowed				= false;
}

//-------------------------------------------------------------------------------
TD3D::~TD3D()
{
	Release();
}

//-------------------------------------------------------------------------------
bool TD3D::IsRunning() const
{
	return m_bRunning;
}

//-------------------------------------------------------------------------------
bool TD3D::WindowedMode() const
{
	return m_g_bWindowed;
}

//-------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 TD3D::GetDevice()
{
	return m_pDevice;
}

//-------------------------------------------------------------------------------
TD3DRENDERSTATE	TD3D::GetShadeMode() const
{
	return m_ShadeMode;
}

//-------------------------------------------------------------------------------
void TD3D::Release()
{
	// Release Vertex Declarations
	SafeRelease(VERTEX::Decl);
	SafeRelease(LVERTEX::Decl);
	SafeRelease(NVERTEX::Decl);
	SafeRelease(Particle::Decl);

	// Release devices last
	SafeRelease(m_pD3D);
	SafeRelease(m_pDevice);
}

//-------------------------------------------------------------------------------
void TD3D::SetScreenSize(int width, int height)
{
	// Usually called before device reset
	d3dpp.BackBufferWidth	= width;
	d3dpp.BackBufferHeight	= height;
}

//-------------------------------------------------------------------------------
bool TD3D::InitDevice(HWND hWnd, int nWidth, int nHeight, int nMinDepth, bool g_bWindowed, int nMinStencil)
{
	if (nMinStencil > 0)
	{
		m_bStencil = true;
	}

	SafeRelease(m_pD3D);	// release if already created

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if (!m_pD3D)
	{
		return false;
	}

	// Get the current display mode
    D3DDISPLAYMODE d3ddm;
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
        return false;
	// Check hardware support for current format (windowed mode)
	if(FAILED(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, d3ddm.Format, true)))
		return false;
	// Check support for fullscreen
	if(FAILED(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false)))
		return false;
	// Check Render Target Support (windowed mode) - should really enumerate through values here
	if(FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, 
		D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_R32F)))
		return false;
	if(FAILED(m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format,
		D3DFMT_R32F, D3DFMT_D24X8)))
		return false;
	// Check Render Target Support (fullscreen)
	if(FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 
		D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, D3DFMT_R32F)))
		return false;
	if(FAILED(m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8,
		D3DFMT_R32F, D3DFMT_D24X8)))
		return false;

	// Set up D3DPRESENT_PARAMETERS
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth			= nWidth;
	d3dpp.BackBufferHeight			= nHeight;
	d3dpp.BackBufferFormat			= d3ddm.Format;
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				= hWnd;
	d3dpp.Windowed					= g_bWindowed;

	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;

	d3dpp.Flags						 = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_ONE;	//D3DPRESENT_INTERVAL_IMMEDIATE

	// Check for hardware T&L
	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	DWORD flags = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// If using hardware then check for pure device
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		  flags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
				flags |= D3DCREATE_PUREDEVICE;

	// Create the Device
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
								hWnd, flags,
								&d3dpp, &m_pDevice)))
	{
		return false;
	}

    // Store render target surface desc
    LPDIRECT3DSURFACE9 pBackBuffer;
    m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &m_pD3DBackBuffer );
	SafeRelease(pBackBuffer);

	// create vertex declarations and set initial render states
	OneTimeInit();

	m_bRunning			= true;
	m_bIsSceneRunning	= false;
	m_g_bWindowed		= g_bWindowed;
	m_ShadeMode			= RS_SHADE_SOLID;	//default value

	return true;

}

//-------------------------------------------------------------------------------
void TD3D::ResetDevice()
{
	m_pDevice->Reset(&d3dpp);
}

//-------------------------------------------------------------------------------
bool TD3D::BeginRendering(bool bClearPixel, bool bClearDepth, bool bClearStencil)
{
	DWORD dw = 0;

	if (bClearPixel) dw |= D3DCLEAR_TARGET;
	if (bClearDepth) dw |= D3DCLEAR_ZBUFFER;
	if (bClearStencil && m_bStencil) dw |= D3DCLEAR_STENCIL;

	if (FAILED(m_pDevice->Clear(0, NULL, dw,
								m_pClearColor,
								1.0f, 0)))
	{
		return false;
	}

	if (FAILED(m_pDevice->BeginScene()))
	{
		return false;
	}

	m_bIsSceneRunning = true;

	return true;
}

//-------------------------------------------------------------------------------
bool TD3D::Clear(bool bClearPixel, bool bClearDepth, bool bClearStencil)
{
	DWORD dw = 0;
	if (bClearPixel) dw |= D3DCLEAR_TARGET;
	if (bClearDepth) dw |= D3DCLEAR_ZBUFFER;
	if (bClearStencil && m_bStencil) dw |= D3DCLEAR_STENCIL;

	if (m_bIsSceneRunning) m_pDevice->EndScene();

	if (FAILED(m_pDevice->Clear(0, NULL, dw,
									m_pClearColor,
									1.0f, 0)))
	{
		return false;
	}

	if (m_bIsSceneRunning)
	{
		m_pDevice->BeginScene();
	}

	return true;
}

//-------------------------------------------------------------------------------
void TD3D::EndRendering()
{
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
	m_bIsSceneRunning = false;
}

//-------------------------------------------------------------------------------
void TD3D::SetClearColor(float fRed, float fGreen, float fBlue)
{
	m_pClearColor = D3DCOLOR_COLORVALUE(fRed, fGreen, fBlue, 1.0f);
}

//-------------------------------------------------------------------------------
void TD3D::SetDepthBufferMode(TD3DRENDERSTATE rs)
{
	if (rs == RS_DEPTH_READWRITE)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	else if (rs == RS_DEPTH_READONLY)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	else if (rs == RS_DEPTH_NONE)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
}

//-------------------------------------------------------------------------------
void TD3D::SetShadeMode(TD3DRENDERSTATE smd, float f)
{
	if (m_ShadeMode == smd)
	{
		//maybe change in size
		if (smd == RS_SHADE_POINTS)
			m_pDevice->SetRenderState(D3DRS_POINTSIZE, *(DWORD*)&f);
		return;
	}

	if (smd == RS_SHADE_TRIWIRE)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_ShadeMode = smd;
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_ShadeMode = smd;
	}

	if (smd == RS_SHADE_POINTS)
	{
		if (f > 0.0f)
		{
			m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_POINTSIZE, *(DWORD*)&f);
			m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, (DWORD)(0.0f));
			m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, (DWORD)(0.0f));
			m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, (DWORD)(0.0f));
			m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, (DWORD)(1.0f));
		}
		else
		{
			m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
		}
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);	
	}
}

//-------------------------------------------------------------------------------
bool TD3D::OneTimeInit()
{
	// Unlit Vertex
	D3DVERTEXELEMENT9 declVertex[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	// Lit Vertex
	D3DVERTEXELEMENT9 declLVertex[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	// Vertex Normal
	D3DVERTEXELEMENT9 declNVertex[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};

	m_pDevice->CreateVertexDeclaration(declVertex, &VERTEX::Decl);
	m_pDevice->CreateVertexDeclaration(declLVertex, &LVERTEX::Decl);
	m_pDevice->CreateVertexDeclaration(declNVertex, &NVERTEX::Decl);
	m_pDevice->SetFVF(NULL);

	// Particle
	D3DVERTEXELEMENT9 ParticleElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		{0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
		{0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};	
	m_pDevice->CreateVertexDeclaration(ParticleElements, &Particle::Decl);

	//Turn on back face culling
    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Turn on Depth Buffering
	m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//Lighting, Color and Blending
	m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	SetClearColor(0.3f, 0.3f, 0.3f);

	//Activate texture filtering
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}

//-------------------------------------------------------------------------------
void TD3D::SetAlphaBlending(bool bEnable)
{
	if (bEnable)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

//-------------------------------------------------------------------------------
void TD3D::SetZFunc(TD3DRENDERSTATE smd)
{
	if (smd == RS_ZFUNC_LESSEQUAL)
	{
		m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}
}