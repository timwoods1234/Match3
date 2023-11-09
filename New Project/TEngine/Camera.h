#ifndef __CAMERA_H__
#define __CAMERA_H__

//+-----------------------------------------------------
//| Game Camera class
//| Supports simple visibility tests
//| Version 1.002
//+-----------------------------------------------------

#ifndef __TVECTOR3_H__
#include "TVector3.h"
#endif

#ifndef __TVECTOR2_H__
#include "TVector2.h"
#endif

#ifndef __TMATRIX_H__
#include "TMatrix.h"
#endif

#ifndef __TPLANE_H__
#include "TPlane.h"
#endif

class TAABB;
class TSphere;

class Camera
{
public:
	Camera();
	~Camera();

	const TMatrix& GetViewMatrix() const;
	const TMatrix& GetProjectionMatrix() const;
	const TMatrix& GetViewProjectionMatrix() const;

	const TVector3& GetRightVec() const;
	const TVector3& GetUpVec() const;
	const TVector3& GetLookVec() const;
	const TVector3& GetPosition() const;
	const TVector2 & GetScreenDimensions() const;
	static const TVector3& Up();

	void Update(const TVector3 & position, const TMatrix* matRotate);
	void LookAt(const TVector3& pos, const TVector3& target, const TVector3& up);
	void SetLens(float fov, float aspect, float nearZ, float farZ);
	void SetScreenSize(float x, float y);
	void SetViewDistance(float dNewDistance);
	void GetBillboardMatrix(TMatrix& matIn) const;

	// "quick swap" methods
	void SetZoom(float zoom);
	void Camera2D();
	void Camera3D();

	// coordinates should be relative to world space.
	bool IsVisible(const TAABB& box) const;
	bool IsVisible(const TVector3& point, float radius) const;
	bool IsVisible(const TSphere& sphere) const;

protected:
	void		BuildView();
	void		BuildWorldFrustumPlanes();

	float		m_fZoom;
	float		m_fViewDistance;
	TVector2	m_screen;

	TMatrix m_matView;
	TMatrix m_matProj;
	TMatrix m_matViewProj;

	// Relative to world space.
	TVector3 m_PosW;
	TVector3 m_RightW;
	TVector3 m_UpW;
	TVector3 m_LookW;

	// Frustum Planes
	TPlane m_frustumPlanes[6];	// [0] = near
	                            // [1] = far
	                            // [2] = left
	                            // [3] = right
	                            // [4] = top
	                            // [5] = bottom
};

#endif
