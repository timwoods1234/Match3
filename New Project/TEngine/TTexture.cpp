#include "TError.h"
#include "TGeneralLib.h"
#include "TTexture.h"
#include "TMemory.h"
#include "Locator.h"
#include "IRenderDevice.h"
#include "SafeRelease.h"

//-------------------------------------------------------------------------------
std::vector<TTexture*> TTexture::ms_pTextures;

//-------------------------------------------------------------------------------
bool TTexture::Load(const char* filename)
{
	TTextureHandle temp;
	return Load(filename, &temp);
}

//-------------------------------------------------------------------------------
bool TTexture::Load(const char* filename, TTextureHandle* nID)
{
	// check for already loaded texture
	for (unsigned int i=0; i<ms_pTextures.size(); ++i)
	{
		if (ms_pTextures[i]->m_strName == filename)
		{
			*nID = i;
			return true;
		}
	}

	LPDIRECT3DTEXTURE9 pD3DTexture = 0;

	// load the texture
	HRESULT hr;
	if (FAILED(hr = D3DXCreateTextureFromFileEx(
					Locator::GetRenderDevice()->GetDevice(),
					filename,
					D3DX_DEFAULT_NONPOW2,	// get width & height from file
					D3DX_DEFAULT_NONPOW2,
					D3DX_FROM_FILE,	// mip levels
					0,				// usage (0 == not render target)
					D3DFMT_A8R8G8B8,// format
					D3DPOOL_MANAGED,
					D3DX_DEFAULT,	// filtering
					D3DX_DEFAULT,	// mip filter
					0,				// disable color key
					NULL,			// extra source info
					NULL,			// palette
					&pD3DTexture)))
	{
		TDebugPrint(("[TTexture::AddTexture] Failed to load %s", filename));
		*nID = -1;
		return false;
	}

	D3DSURFACE_DESC desc;
	pD3DTexture->GetLevelDesc(0, &desc);

	TTexture* pNewTexture = TNew TTexture();

	pNewTexture->m_pTexture = pD3DTexture;
	pNewTexture->m_strName  = filename;
	pNewTexture->m_nWidth	= desc.Width;
	pNewTexture->m_nHeight	= desc.Height;

	*nID = ms_pTextures.size();
	ms_pTextures.push_back(pNewTexture);

	return true;
}

//-------------------------------------------------------------------------------
void TTexture::Unload()
{
	for (unsigned int i=0; i<ms_pTextures.size(); ++i)
	{
		TSafeDelete(ms_pTextures[i]);
	}

	ms_pTextures.clear();
}

//-------------------------------------------------------------------------------
bool TTexture::PreLoad(const char* filename)
{
	// open file
	FILE* f;
	fopen_s(&f, filename, "r");

	if (!f)
	{
		TDebugPrint(("[TTexture::PreLoad] Couldn't open file (%s)", filename));
		return false;
	}

	// read lines until EOF
	char str[1024];

	int nLine = 0;

	while (fgets(str, 1024, f))
	{
		// fetch next line
		std::string string = str;		// convert to std::string (easier)
		string.erase( string.end()-1 );	// cut newline

		if (!Load(string.c_str()))
		{
			TDebugPrint(("[TTexture::PreLoad]: preload failed (%s)", string.c_str()));
		}
		++nLine;
	}

	fclose(f);
	f = NULL;

	return true;
}

//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9& TTexture::GetTexture(const TString& filename)
{
	for (unsigned int i=0; i<ms_pTextures.size(); ++i)
	{
		if (ms_pTextures[i]->m_strName == filename)
			return ms_pTextures[i]->m_pTexture;
	}

	return ms_pTextures[0]->m_pTexture;
}

//-------------------------------------------------------------------------------
TTextureHandle TTexture::GetTextureHandle(const TString& filename)
{
	for (unsigned int i=0; i<ms_pTextures.size(); ++i)
	{
		if (ms_pTextures[i]->m_strName == filename)
		{
			return i;
		}
	}

	TDebugPrint(("[TTexture::GetTextureHandle] couldn't find %s", *filename));
	return -1;
}

//-------------------------------------------------------------------------------
const TTexture* TTexture::GetTextureInfo(unsigned int nID)
{
	TASSERT( (nID >= 0 && nID < ms_pTextures.size()), "[TTexture::GetTextureInfo] invalid index");
	return ms_pTextures[nID];
}

//-------------------------------------------------------------------------------
TTexture::TTexture()
:	m_pTexture(0)
{
}

//-------------------------------------------------------------------------------
TTexture::~TTexture()
{
	SafeRelease(m_pTexture);
}
