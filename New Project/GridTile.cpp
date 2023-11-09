#include "GridTile.h"
#include "SpritePlugin.h"

//-------------------------------------------------------------------------------
GridTile::GridTile()
:	m_pEntity(NULL),
	m_nX(0),
	m_nY(0),
	m_bMatched(false)
{
}

//-------------------------------------------------------------------------------
GridTile::~GridTile()
{
	m_pEntity = NULL;
}

//-------------------------------------------------------------------------------
Entity* GridTile::GetEntity() const
{
	return m_pEntity;
}

//-------------------------------------------------------------------------------
void GridTile::SetEntity(Entity* pEntity)
{
	m_pEntity = pEntity;
}

//-------------------------------------------------------------------------------
void GridTile::SetXY(int x, int y)
{
	m_nX = x;
	m_nY = y;
}

//-------------------------------------------------------------------------------
int GridTile::GetX() const
{
	return m_nX;
}

//-------------------------------------------------------------------------------
int GridTile::GetY() const
{
	return m_nY;
}

//-------------------------------------------------------------------------------
bool GridTile::Matches(const GridTile& other) const
{
	static const TString spriteStr = "Sprite";

	if (m_pEntity == NULL || other.GetEntity() == NULL)
	{
		return false;
	}

	SpritePlugin* mySprite = static_cast<SpritePlugin*>(m_pEntity->GetPlugin(spriteStr));
	SpritePlugin* otherSprite = static_cast<SpritePlugin*>(other.m_pEntity->GetPlugin(spriteStr));

	return mySprite->GetSprite()->GetFilename() == otherSprite->GetSprite()->GetFilename();
}

//-------------------------------------------------------------------------------
bool GridTile::IsAdjecent(const GridTile& other) const
{
	int x = m_nX - other.m_nX;
	int y = m_nY - other.m_nY;

	if ((x == 1 || x == -1) && (y == 0))
	{
		return true;
	}
	else if ((y == 1 || y == -1) && (x == 0))
	{
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------
bool GridTile::operator==(const GridTile& other) const
{
	return m_nX == other.m_nX && m_nY == other.m_nY;
}

//-------------------------------------------------------------------------------
bool GridTile::operator!=(const GridTile& other) const
{
	return !(*this == other);
}

//-------------------------------------------------------------------------------
bool GridTile::IsMatched() const
{
	return m_bMatched;
}

//-------------------------------------------------------------------------------
void GridTile::SetMatched(bool matched)
{
	m_bMatched = matched;
}
