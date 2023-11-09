//-------------------------------------------------------------------------------
inline void	TMesh::GetAABB(TAABB& box) const
{
	box = m_aabbBounds;
}

//-------------------------------------------------------------------------------
inline void	TMesh::GetSphere(TSphere& sphere) const
{
	sphere = m_sphereBounds;
}

//-------------------------------------------------------------------------------
D3DXMATERIAL & TMesh::GetMaterial(unsigned int nSubset) const
{
	return m_pMaterials[nSubset];
}

//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 & TMesh::GetTexture(unsigned int nSubset) const
{
	return TTexture::GetTexture(m_pTextures[nSubset]);
}

//-------------------------------------------------------------------------------
DWORD TMesh::GetNumMaterials() const
{
	return m_nNumMaterials;
}

//-------------------------------------------------------------------------------
bool TMesh::SetTexture(unsigned int nSubset, TTextureHandle nTexture)
{
	TASSERT( (nSubset >= 0 && nSubset < m_nNumMaterials), "[TMesh::SetTexture] invalid index");

	m_pTextures[nSubset] = nTexture;
	return true;
}

//-------------------------------------------------------------------------------
void TMesh::DrawSubset(unsigned int nSubset)
{
	m_pMesh->DrawSubset(nSubset);
}

//-------------------------------------------------------------------------------
const TString& TMesh::GetFilename() const
{
	return m_strFilename;
}

//-------------------------------------------------------------------------------
DWORD TMesh::GetNumTriangles() const
{
	return m_pMesh->GetNumFaces();
}

//-------------------------------------------------------------------------------
DWORD TMesh::GetNumVertices() const
{
	return m_pMesh->GetNumVertices();
}

//-------------------------------------------------------------------------------
UINT TMesh::GetMemoryFootprint() const
{
	return m_nMemoryFootprint;
}

//-------------------------------------------------------------------------------
TMesh* TMesh::GetMesh(TMeshHandle nID)
{
	TASSERT( (nID >= 0 && nID < ms_pMeshes.size()), "[TMesh::GetMesh] invalid index");
	return ms_pMeshes[nID];
}
