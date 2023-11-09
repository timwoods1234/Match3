#ifndef __D3DRENDERER_H__
#define __D3DRENDERER_H__

//+-------------------------------------------------------
//| D3D Rendering component.
//| Version 1.008
//+-------------------------------------------------------

#include "IRenderer.h"

class D3DRenderer : public IRenderer
{
public:
	D3DRenderer();
	virtual ~D3DRenderer();

	void OnLostDevice();
	void OnResetDevice();

	void Draw3D(unsigned int nMesh, const TMatrix &mWorld);

	void Draw2D(unsigned int asset, const TVector2& position, 
				float rotation=0.f, float scaleX=1.f, float scaleY=1.f);

	void DrawBillboard(UINT nTexture, const TMatrix& mWorld);

	void Begin(unsigned int nEffect);
	void BeginPass(unsigned int nEffect, unsigned int nPass);
	void End(unsigned int nEffect);
	void EndPass(unsigned int nEffect);
	void Camera3D();
	void Camera2D();

private:
	struct ID3DXEffect* m_pFX;		// General light/texture FX
	const char*			m_hTech;
	const char*			m_hWVP;
	const char*			m_hEyePosW;
	const char*			m_hWorld;
	const char*			m_hTex;
	const char*			m_hMtrl;
};

#endif
