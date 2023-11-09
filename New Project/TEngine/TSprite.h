#ifndef __TSPRITE_H__
#define __TSPRITE_H__

//+-------------------------------------------------------
//| Wrapper class for 2D sprites.
//| Version 1.000
//| All code (c) 2013 Tim Woods. All rights reserved.
//+-------------------------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef __TMathClasses_H__
#include "TMathClasses.h"
#endif

#ifndef __TTEXTURE_H__
#include "TTexture.h"
#endif

#ifndef __TSTRING_H__
#include "TString.h"
#endif

class TSprite
{
public:
	TSprite();
	~TSprite();

	bool LoadTexture(const char* filename, const TVector2 * dimensions = 0);
	void SetTexture(UINT nTexture, const TVector2 * dimensions = 0);
	TTextureHandle GetTextureHandle() const;
	const TString& GetFilename() const;
	const TVector2& GetDimensions() const;
	void SetDimensions(const TVector2& dimensions);

	void Render(const TVector2 & position, float rotation=0.0, float scaleX=1.0, float scaleY=1.0);

protected:
	TTextureHandle m_texture;
	TVector2 m_dimensions;

	TString m_strFilename;
};

#endif