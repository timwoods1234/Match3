#ifndef __IRENDERDEVICE_H__
#define __IRENDERDEVICE_H__

//+-------------------------------------------------------
//| Interface to a rendering device.
//| Version 1.000
//+-------------------------------------------------------

#ifndef __TUTIL_H__
#include "TUtil.h"
#endif

class IRenderDevice
{
public:
	IRenderDevice() {}
	virtual ~IRenderDevice() {}

	virtual void				ResetDevice() = 0;
	virtual bool				IsRunning() const = 0;
	virtual void				SetScreenSize(int width, int height) = 0;
	virtual bool				WindowedMode() const = 0;

	virtual LPDIRECT3DDEVICE9	GetDevice() = 0;

	// Rendering
	virtual bool				BeginRendering(bool bClearPixel, bool bClearDepth, bool bClearStencil) = 0;
	virtual bool				Clear(bool bClearPixel, bool bClearDepth, bool bClearStencil) = 0;
	virtual void				EndRendering() = 0;
	virtual void				SetClearColor(float fRed, float fGreen, float fBlue) = 0;

	virtual void				SetDepthBufferMode(TD3DRENDERSTATE rs) = 0;
	virtual void				SetShadeMode(TD3DRENDERSTATE smd, float f) = 0;
	virtual TD3DRENDERSTATE		GetShadeMode() const = 0;
	virtual void				SetAlphaBlending(bool bEnable) = 0;
	virtual void				SetZFunc(TD3DRENDERSTATE smd) = 0;
};

#endif
