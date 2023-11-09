//-------------------------------------------------------------------------------
unsigned int TTexture::GetWidth() const
{
	return m_nWidth;
}

//-------------------------------------------------------------------------------
unsigned int TTexture::GetHeight() const
{
	return m_nHeight;
}

//-------------------------------------------------------------------------------
const TString& TTexture::GetFilename() const
{
	return m_strName;
}

//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9& TTexture::GetTexture(TTextureHandle nID)
{
	return ms_pTextures[nID]->m_pTexture;
}