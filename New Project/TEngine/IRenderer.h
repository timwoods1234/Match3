#ifndef __IRENDERER_H__
#define __IRENDERER_H__

//+-------------------------------------------------------
//| Interface for the rendering component.
//| Version 1.000
//+-------------------------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

class TMatrix;
class TVector2;
class Camera;

#define TBILLBOARD	0

class IRenderer
{
public:
	IRenderer();
	virtual ~IRenderer();

	virtual void Camera3D();
	virtual void Camera2D();

	void SetCurrentCamera(Camera* pCamera);
	Camera* GetCurrentCamera() const;

	void  SetNumTriangles(DWORD n);
	DWORD GetNumTriangles() const;

	void  SetNumVertices(DWORD n);
	DWORD GetNumVertices() const;

	// must be overridden
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;
	virtual void Draw3D(UINT nMesh, const TMatrix& mWorld) = 0;
	virtual void Draw2D(UINT asset, const TVector2& position,
						float rotation=0.f, float scaleX=1.f, float scaleY=1.f) = 0;
	virtual void DrawBillboard(UINT nTexture, const TMatrix& mWorld) = 0;
	
	// may be overridden
	virtual void Begin(UINT nEffect) {}
	virtual void BeginPass(UINT nEffect, UINT nPass) {}
	virtual void End(UINT nEffect) {}
	virtual void EndPass(UINT nEffect) {}

protected:
	DWORD	m_nNumTriangles;
	DWORD	m_nNumVertices;
	Camera* m_pCurrentCamera;
};

#endif