#include "Camera.h"
#include "Collision.h"
#include "TSphere.h"

//-------------------------------------------------------------------------------
const TVector3& Camera::Up()
{
	static TVector3 up(0.f, 1.f, 0.f);
	return up;
}

//-------------------------------------------------------------------------------
Camera::Camera()
: m_fZoom(1.0f),
  m_PosW(0.0f, 0.0f, 0.0f),
  m_RightW(1.0f, 0.0f, 0.0f),
  m_UpW(0.0f, 1.0f, 0.0f),
  m_LookW(0.0f, 0.0f, 1.0f),
  m_screen(10.0f, 10.0f),
  m_fViewDistance(2000.0f)
{
	m_matView.SetIdentity();
	m_matProj.SetIdentity();
	m_matViewProj.SetIdentity();
}

//-------------------------------------------------------------------------------
Camera::~Camera()
{

}

//-------------------------------------------------------------------------------
const TMatrix& Camera::GetViewMatrix() const
{
	return m_matView;
}

//-------------------------------------------------------------------------------
const TMatrix& Camera::GetProjectionMatrix() const
{
	return m_matProj;
}

//-------------------------------------------------------------------------------
const TMatrix& Camera::GetViewProjectionMatrix() const
{
	return m_matViewProj;
}

//-------------------------------------------------------------------------------
const TVector3& Camera::GetRightVec() const
{
	return m_RightW;
}

//-------------------------------------------------------------------------------
const TVector3& Camera::GetUpVec() const
{
	return m_UpW;
}

//-------------------------------------------------------------------------------
const TVector3& Camera::GetLookVec() const
{
	return m_LookW;
}

//-------------------------------------------------------------------------------
const TVector3& Camera::GetPosition() const
{
	return m_PosW;
}

//-------------------------------------------------------------------------------
const TVector2 & Camera::GetScreenDimensions() const
{
	return m_screen;
}

//-------------------------------------------------------------------------------
void Camera::SetScreenSize(float x, float y)
{
	m_screen.x = x;
	m_screen.y = y;
}

//-------------------------------------------------------------------------------
void Camera::SetZoom(float zoom)
{
	m_fZoom = zoom;
}

//-------------------------------------------------------------------------------
void Camera::Update(const TVector3 & position, const TMatrix* matRotate)
{
	TVector3 vector = TVector3(0.f, 0.f, 1.f);
	matRotate->TransformVector(&vector);

	TVector3 vUpVec = TVector3(0.f, 1.f, 0.f);
	matRotate->TransformVector(&vUpVec);

	TVector3 fromPt = position + (vUpVec * 1);
	fromPt -= vector * m_fZoom;

	TVector3 lookAtPt = position + vector;

	LookAt( fromPt, lookAtPt, vUpVec );
	return;
}

//-------------------------------------------------------------------------------
void Camera::LookAt(const TVector3& pos, const TVector3& target, const TVector3& up)
{
	TVector3 L = target - pos;
	L.Normalize();

	TVector3 R = CrossProduct(up, L);
	R.Normalize();

	TVector3 U = CrossProduct(L, R);
	U.Normalize();

	m_PosW   = pos;
	m_RightW = R;
	m_UpW    = U;
	m_LookW  = L;

	BuildView();
	BuildWorldFrustumPlanes();

	m_matViewProj = m_matView * m_matProj;
}

//-------------------------------------------------------------------------------
void Camera::SetLens(float fov, float aspect, float nearZ, float farZ)
{
	m_matProj.SetPerspectiveProjection(fov, aspect, nearZ, farZ);

	BuildWorldFrustumPlanes();

	m_matViewProj = m_matView * m_matProj;
}

//-------------------------------------------------------------------------------
void Camera::Camera2D()
{
	m_matProj.SetOrthoProjection(m_screen.x, m_screen.y, -100.f, 100.f);

	LookAt( TVector3(m_screen.x*0.5f,-m_screen.y*0.5f,-0.1f),
			TVector3(m_screen.x*0.5f,-m_screen.y*0.5f,0),
			TVector3(0,1,0) );
}

//-------------------------------------------------------------------------------
void Camera::Camera3D()
{
	// Aspect ratio depends on the screen dimensions
	SetLens(PI * 0.25f, m_screen.x/m_screen.y, 1.0f, m_fViewDistance);
}

//-------------------------------------------------------------------------------
void Camera::BuildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	m_LookW.Normalize();

	m_UpW = CrossProduct(m_LookW, m_RightW);
	m_UpW.Normalize();

	m_RightW = CrossProduct(m_UpW, m_LookW);
	m_RightW.Normalize();

	// Fill in the view matrix entries.
	float x = -DotProduct(m_PosW, m_RightW);
	float y = -DotProduct(m_PosW, m_UpW);
	float z = -DotProduct(m_PosW, m_LookW);

	m_matView.m[0][0] = m_RightW.x; 
	m_matView.m[1][0] = m_RightW.y; 
	m_matView.m[2][0] = m_RightW.z; 
	m_matView.m[3][0] = x;   

	m_matView.m[0][1] = m_UpW.x;
	m_matView.m[1][1] = m_UpW.y;
	m_matView.m[2][1] = m_UpW.z;
	m_matView.m[3][1] = y;  

	m_matView.m[0][2] = m_LookW.x; 
	m_matView.m[1][2] = m_LookW.y; 
	m_matView.m[2][2] = m_LookW.z; 
	m_matView.m[3][2] = z;   

	m_matView.m[0][3] = 0.0f;
	m_matView.m[1][3] = 0.0f;
	m_matView.m[2][3] = 0.0f;
	m_matView.m[3][3] = 1.0f;
}

//-------------------------------------------------------------------------------
bool Camera::IsVisible(const TAABB& box)const
{
	for(int i=0; i<6; ++i)
	{
		if (CullingTest(box, m_frustumPlanes[i]) == CULLING_OUTSIDE)
		{
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------
bool Camera::IsVisible(const TVector3& point, float radius) const
{
	TSphere testSphere(point, radius);
	return IsVisible(testSphere);
}

//-------------------------------------------------------------------------------
bool Camera::IsVisible(const TSphere& sphere) const
{
	for(int i=0; i<6; ++i)
	{
		if (CullingTest(sphere, m_frustumPlanes[i]) == CULLING_OUTSIDE)
		{
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------
void Camera::BuildWorldFrustumPlanes()
{
	// Note: Extract the frustum planes in world space.

	TMatrix VP = m_matView * m_matProj;

	TVector4 col0(VP.m[0][0], VP.m[1][0], VP.m[2][0], VP.m[3][0]);
	TVector4 col1(VP.m[0][1], VP.m[1][1], VP.m[2][1], VP.m[3][1]);
	TVector4 col2(VP.m[0][2], VP.m[1][2], VP.m[2][2], VP.m[3][2]);
	TVector4 col3(VP.m[0][3], VP.m[1][3], VP.m[2][3], VP.m[3][3]);

	// Planes face inward.
	m_frustumPlanes[0] = (TPlane)(col2);        // near
	m_frustumPlanes[1] = (TPlane)(col3 - col2); // far
	m_frustumPlanes[2] = (TPlane)(col3 + col0); // left
	m_frustumPlanes[3] = (TPlane)(col3 - col0); // right
	m_frustumPlanes[4] = (TPlane)(col3 - col1); // top
	m_frustumPlanes[5] = (TPlane)(col3 + col1); // bottom

	for(int i = 0; i < 6; i++)
	{
		Normalize(m_frustumPlanes[i]);
	}
}

//-------------------------------------------------------------------------------
void Camera::SetViewDistance(float fNewDistance)
{
	m_fViewDistance = fNewDistance;
}

//-------------------------------------------------------------------------------
void Camera::GetBillboardMatrix(TMatrix& matIn) const
{
	// invert the 3x3 part (rotation) of the view to give billboard matrix
	matIn.Set(m_matView[0][0], m_matView[1][0], m_matView[2][0], 0.f,
			  m_matView[0][1], m_matView[1][1], m_matView[2][1], 0.f,
			  m_matView[0][2], m_matView[1][2], m_matView[2][2], 0.f,
			  0.f,			   0.f,				0.f,			 1.f);
}