#include "IRenderer.h"
#include "Camera.h"

//-------------------------------------------------------------------------------
IRenderer::IRenderer()
:	m_nNumTriangles(0),
	m_nNumVertices(0),
	m_pCurrentCamera(0)
{

}

//-------------------------------------------------------------------------------
IRenderer::~IRenderer()
{

}

//-------------------------------------------------------------------------------
void  IRenderer::SetNumTriangles(DWORD n)
{
	m_nNumTriangles = n;
}

//-------------------------------------------------------------------------------
DWORD IRenderer::GetNumTriangles() const
{
	return m_nNumTriangles;
}

//-------------------------------------------------------------------------------
void  IRenderer::SetNumVertices(DWORD n)
{
	m_nNumVertices = n;
}

//-------------------------------------------------------------------------------
DWORD IRenderer::GetNumVertices() const
{
	return m_nNumVertices;
}

//-------------------------------------------------------------------------------
void IRenderer::Camera2D()
{
	m_pCurrentCamera->Camera2D();
}

//-------------------------------------------------------------------------------
void IRenderer::Camera3D()
{
	m_pCurrentCamera->Camera3D();
}

//-------------------------------------------------------------------------------
void IRenderer::SetCurrentCamera(Camera* pCamera)
{
	m_pCurrentCamera = pCamera;
}

//-------------------------------------------------------------------------------
Camera* IRenderer::GetCurrentCamera() const
{
	return m_pCurrentCamera;
}
