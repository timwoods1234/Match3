#include "TEngine.h"
#include "D3DRenderer.h"

//-------------------------------------------------------------------------------
D3DRenderer::D3DRenderer()
{
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(Locator::GetRenderDevice()->GetDevice(), "Assets/Data/LitTex.fx", 
		0, 0, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, 0, &m_pFX, &errors);
	if( errors )
	{
		Log::Get()->Print((char*)errors->GetBufferPointer());
		TASSERT(false, "[D3DRenderer] Failed to load shader");
	}

	m_hTech            = "LitTex";
	m_hWVP             = "gWVP";
	m_hEyePosW         = "gEyePosW";
	m_hWorld           = "gWorld";
	m_hMtrl            = "gMtrl";
	m_hTex             = "gTex";
}

//-------------------------------------------------------------------------------
D3DRenderer::~D3DRenderer()
{
	SafeRelease(m_pFX);
}

//-------------------------------------------------------------------------------
void D3DRenderer::Draw3D(UINT nMesh, const TMatrix &mWorld)
{
	m_pFX->SetValue(m_hWVP, &(mWorld * m_pCurrentCamera->GetViewProjectionMatrix()), sizeof(TMatrix));
	m_pFX->SetValue(m_hWorld, &mWorld, sizeof(TMatrix));

	TMesh* pMesh = TMesh::GetMesh(nMesh);

	for (unsigned int i=0; i<pMesh->GetNumMaterials(); ++i)
	{
		// material & texture
		m_pFX->SetValue(m_hMtrl, &(pMesh->GetMaterial(i).MatD3D), sizeof(D3DMATERIAL9));

		m_pFX->SetTexture(m_hTex, pMesh->GetTexture(i));

		// draw subset
		m_pFX->CommitChanges();
		pMesh->DrawSubset(i);
	}

	m_nNumVertices += pMesh->GetNumVertices();
	m_nNumTriangles += pMesh->GetNumTriangles();
}

//-------------------------------------------------------------------------------
void D3DRenderer::Draw2D(UINT asset, const TVector2& position, float rotation, float scaleX, float scaleY)
{
	// insert scale, and rotation if valid
	if (rotation == 0.f)
	{
		g_matWorld[0] = TVector4(scaleX, 0.f, 0.f, 0.f);
		g_matWorld[1] = TVector4(0.f, scaleY, 0.f, 0.f);
		g_matWorld[2] = TVector4(0.f, 0.f, 128.f, 0.f);
	}
	else
	{
		float sinAngle = 1.f;
		float cosAngle = 1.f;

		SinCos(rotation, sinAngle, cosAngle);

		g_matWorld[0] = TVector4(scaleX * cosAngle, scaleX * sinAngle, 0.f, 0.f);
		g_matWorld[1] = TVector4(scaleY * -sinAngle, scaleY * cosAngle, 0.f, 0.f);
		g_matWorld[2] = TVector4(0.f, 0.f, 128.f, 0.f);
	}

	// insert translation
	g_matWorld[3][0] = position.x+ scaleX * 0.5f;
	g_matWorld[3][1] = (-1 * position.y)- scaleY * 0.5f;
	g_matWorld[3][2] = 0.f;
	g_matWorld[3][3] = 1.f;

	// eye/world
	m_pFX->SetValue(m_hWVP, &(g_matWorld * m_pCurrentCamera->GetViewProjectionMatrix()), sizeof(TMatrix));
	m_pFX->SetValue(m_hWorld, &g_matWorld, sizeof(TMatrix));

	// material
	m_pFX->SetValue(m_hMtrl,
		&(TMesh::GetMesh(g_nGUI)->GetMaterial(0).MatD3D), sizeof(D3DMATERIAL9));

	// texture
	m_pFX->SetTexture(m_hTex, TTexture::GetTexture(asset));

	// done
	m_pFX->CommitChanges();
	TMesh::GetMesh(g_nGUI)->DrawSubset(0);
}

//-------------------------------------------------------------------------------
void D3DRenderer::Begin(UINT nEffect)
{
	UINT nNumPasses = 0;
	m_pFX->Begin(&nNumPasses, 0);
}

//-------------------------------------------------------------------------------
void D3DRenderer::BeginPass(UINT nEffect, UINT nPass)
{
	m_pFX->BeginPass(nPass);
}

//-------------------------------------------------------------------------------
void D3DRenderer::End(UINT nEffect)
{
	m_pFX->End();
}

//-------------------------------------------------------------------------------
void D3DRenderer::EndPass(UINT nEffect)
{
	m_pFX->EndPass();
}

//-------------------------------------------------------------------------------
void D3DRenderer::Camera2D()
{
	IRenderer::Camera2D();
	m_pFX->SetValue(m_hEyePosW, &m_pCurrentCamera->GetPosition(), sizeof(TVector3));
}

//-------------------------------------------------------------------------------
void D3DRenderer::Camera3D()
{
	IRenderer::Camera3D();
	m_pFX->SetValue(m_hEyePosW, &m_pCurrentCamera->GetPosition(), sizeof(TVector3));
}

//-------------------------------------------------------------------------------
void D3DRenderer::OnLostDevice()
{
	m_pFX->OnLostDevice();
}

//-------------------------------------------------------------------------------
void D3DRenderer::OnResetDevice()
{
	m_pFX->OnResetDevice();
	m_pFX->SetTechnique(m_hTech);
	m_pFX->CommitChanges();
}

//-------------------------------------------------------------------------------
void D3DRenderer::DrawBillboard(UINT nTexture, const TMatrix& mWorld)
{
	TMesh::GetMesh(TBILLBOARD)->SetTexture(0, nTexture);
	Draw3D(TBILLBOARD, mWorld);
}
