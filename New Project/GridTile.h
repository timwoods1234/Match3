#ifndef __GRIDTILE_H__
#define __GRIDTILE_H__

//+-------------------------------------------------------
//| Object that represents a single tile
//| Contains a pointer to an entity
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class GridTile
{
public:
	GridTile();
	~GridTile();

	Entity* GetEntity() const;
	void SetEntity(Entity* pEntity);
	void SetXY(int x, int y);
	int GetX() const;
	int GetY() const;
	bool IsMatched() const;
	void SetMatched(bool matched);

	bool Matches(const GridTile& other) const;
	bool operator==(const GridTile& other) const;
	bool operator!=(const GridTile& other) const;
	bool IsAdjecent(const GridTile& other) const;

private:
	Entity* m_pEntity;
	int m_nX;
	int m_nY;
	bool m_bMatched;
};

#endif
