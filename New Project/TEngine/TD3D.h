#ifndef __TD3D_H__
#define __TD3D_H__

//+-------------------------------------------------------
//| D3DX9 Rendering Component
//| Version 1.008
//+-------------------------------------------------------

#include "IRenderDevice.h"

class TD3D : public IRenderDevice
{
public:
	TD3D();
	~TD3D();

	bool				InitDevice(HWND hWnd,
									int nWidth, int nHeight, int nMinDepth=1,
									bool g_bWindowed=true, int nMinStencil=0
									);

	bool				WindowedMode() const;
	void				ResetDevice();
	bool				IsRunning() const;
	void				SetScreenSize(int width, int height);
	LPDIRECT3DDEVICE9	GetDevice();

	//+-----------------------------------------------------------------------
	//| Rendering
	//+-----------------------------------------------------------------------
	bool				BeginRendering(bool bClearPixel, bool bClearDepth, bool bClearStencil);
	bool				Clear(bool bClearPixel, bool bClearDepth, bool bClearStencil);
	void				EndRendering();
	void				SetClearColor(float fRed, float fGreen, float fBlue);

	void				SetDepthBufferMode(TD3DRENDERSTATE rs);
	void				SetShadeMode(TD3DRENDERSTATE smd, float f);
	TD3DRENDERSTATE		GetShadeMode() const;
	void				SetAlphaBlending(bool bEnable);
	void				SetZFunc(TD3DRENDERSTATE smd);

private:
	bool					OneTimeInit();
	void					Release();

	LPDIRECT3DDEVICE9		m_pDevice;

	bool					m_bRunning;
	bool					m_g_bWindowed;
	bool					m_bIsSceneRunning;
	bool					m_bStencil;
	LPDIRECT3D9				m_pD3D;

	D3DPRESENT_PARAMETERS	d3dpp;
	D3DSURFACE_DESC			m_pD3DBackBuffer;
	TD3DRENDERSTATE			m_ShadeMode;
	D3DCOLOR				m_pClearColor;
}; // class TD3D

#endif
