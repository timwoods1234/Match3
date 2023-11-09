#include "TSprite.h"
#include "IRenderer.h"
#include "Locator.h"
#include "TError.h"

//-------------------------------------------------------------------------------
TSprite::TSprite()
:	m_texture(0),
	m_dimensions(32.f, 32.f)
{
	m_strFilename = "";
}

//-------------------------------------------------------------------------------
TSprite::~TSprite()
{

}

//-------------------------------------------------------------------------------
bool TSprite::LoadTexture(const char* filename, const TVector2* dimensions)
{
	if (!(TTexture::Load(filename, &m_texture)))
	{
		TDebugPrint(("[TSprite::LoadTexture] Failed to load %s", filename));
		return false;
	}

	if (dimensions)
	{
		m_dimensions = *dimensions;
	}
	else
	{
		// get size from texture if not specificed
		m_dimensions.x = (float)TTexture::GetTextureInfo(m_texture)->GetWidth();
		m_dimensions.y = (float)TTexture::GetTextureInfo(m_texture)->GetHeight();
	}

	m_strFilename = filename;
	return true;
}

//-------------------------------------------------------------------------------
void TSprite::Render(const TVector2& position, float rotation, float scaleX, float scaleY)
{
	Locator::GetRenderer()->Draw2D( m_texture, 
									position, 
									rotation, 
									m_dimensions.x*scaleX, 
									m_dimensions.y*scaleY);
}

//-------------------------------------------------------------------------------
void TSprite::SetTexture(UINT nTexture, const TVector2* dimensions)
{
	m_texture = nTexture;

	if (dimensions)
	{
		m_dimensions = *dimensions;
	}
}

//-------------------------------------------------------------------------------
const TVector2 & TSprite::GetDimensions() const
{
	return m_dimensions;
}

//-------------------------------------------------------------------------------
TTextureHandle TSprite::GetTextureHandle() const
{
	return m_texture;
}

//-------------------------------------------------------------------------------
void TSprite::SetDimensions(const TVector2& dimensions)
{
	m_dimensions = dimensions;
}

//-------------------------------------------------------------------------------
const TString& TSprite::GetFilename() const
{
	return m_strFilename;
}
