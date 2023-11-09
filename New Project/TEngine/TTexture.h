#ifndef __TTEXTURE_H__
#define __TTEXTURE_H__

//+-------------------------------------------------------
//| texture class, wraps d3d textures
//| contains a static array of textures indexed by filename
//| all textures loaded only once
//| Version 1.000
//+-------------------------------------------------------

#ifndef __D3DX9_H__
#include <d3dx9.h>
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __TVECTOR2_H__
#include "TVector2.h"
#endif

#ifndef __TTEXTUREHANDLEDEFINED__
#define __TTEXTUREHANDLEDEFINED__
#define TTextureHandle unsigned int
#endif

class TTexture
{
public:
	// load by filename and optionally gain a handle to the loaded texture
	static bool Load(const char* filename);
	static bool Load(const char* filename, TTextureHandle* nID);

	// delete all loaded textures
	static void Unload();

	// load several textures from a file
	static bool PreLoad(const char* filename);

	// direct access to texture
	static inline LPDIRECT3DTEXTURE9& GetTexture(TTextureHandle nID);
	static LPDIRECT3DTEXTURE9&		  GetTexture(const TString& filename);

	// find a texture handle from the filename
	static TTextureHandle		GetTextureHandle(const TString& filename);

	// get filename, height, width, etc
	static const TTexture*		GetTextureInfo(TTextureHandle nID);

	TTexture();
	~TTexture();

	inline unsigned int		GetWidth() const;
	inline unsigned int		GetHeight() const;
	inline const TString&	GetFilename() const;

private:
	// currently loaded textures
	static std::vector<TTexture*> ms_pTextures;

	TString				m_strName;
	LPDIRECT3DTEXTURE9	m_pTexture;
	unsigned int		m_nWidth;
	unsigned int		m_nHeight;
};

#include "TTexture.inl"

#endif
