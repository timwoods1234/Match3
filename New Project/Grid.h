#ifndef __GRID_H__
#define __GRID_H__

//+-------------------------------------------------------
//| The game grid. Contains an array of tiles
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class GridTile;

class Grid : public Singleton<Grid>, public TEventListener
{
public:
	Grid();
	~Grid();

	GridTile* GetTile(const TVector2& screenPosition) const;
	void SwapTiles(GridTile* a, GridTile* b);
	void OnMoveComplete();
	void Reset();
	int GetTileSize() const;
	const TVector2& GetOffset() const;

private:
	GridTile* GetTile(int x, int y) const;
	void ScreenToGrid(const TVector2& screenPos, int& x, int& y) const;
	bool CheckForCombo();
	bool CheckLine(int startX, int startY, int offsetX, int offsetY, int minCount);
	void ApplyCombo(GridTile* pStart, int count, int offsetX, int offsetY);
	void AddMatchedTile(GridTile* pTile);
	void TidyColumn(int column);

	struct Swap
	{
		GridTile* pStart;
		GridTile* pEnd;
	};

	std::vector<GridTile*> m_pMatchedTiles;
	std::vector<int> m_matchedColumns;

	static const int GRID_SIZE = 8;

	int			m_nTileSize;
	int			m_nComboCount;
	int			m_nScorePerTile;
	float		m_fTransitionTime;
	Swap		m_lastSwap;
	GridTile*	m_pTiles[GRID_SIZE * GRID_SIZE];
	TVector2	m_offset;
	EntityDefinition* m_pDefinition;
	TSoundHandle m_comboSound;
};

DefineSingleton(Grid);

#endif
