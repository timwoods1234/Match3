#include "SpritePlugin.h"

#include "Grid.h"
#include "GridTile.h"
#include "MoverPlugin.h"
#include "BouncerPlugin.h"
#include "PluginFactory.h"
#include "EventMoveFinished.h"
#include "SpriteAssigner.h"
#include "Player.h"
#include "EventCombo.h"

//-------------------------------------------------------------------------------
static const TString moverStr = "Mover";

//-------------------------------------------------------------------------------
Grid::Grid()
:	m_nTileSize(35),
	m_nComboCount(2),
	m_offset(32,32),
	m_fTransitionTime(0.5f),
	m_nScorePerTile(1)
{
	DataLoader settings;
	settings.LoadFile("Assets/Data/Settings.txt");

	settings.GetInt("TileSize", m_nTileSize, true);
	settings.GetInt("ComboCount", m_nComboCount, true);
	settings.GetInt("ScorePerTile", m_nScorePerTile, true);
	settings.GetFloat("GridX", m_offset.x, true);
	settings.GetFloat("GridY", m_offset.y, true);
	settings.GetFloat("TransitionTime", m_fTransitionTime, true);

	int index = 0;

	for (int i=0; i<GRID_SIZE; i++)
	{
		for (int j=0; j<GRID_SIZE; j++)
		{
			m_pTiles[index] = TNew GridTile();

			m_pTiles[index]->SetXY(j, i);

			index++;
		}
	}

	m_pDefinition = CreateDefinition();

	for (int i=0; i< GRID_SIZE*GRID_SIZE; ++i)
	{
		Entity* pNewEntity = CreateEntity(m_pDefinition);
		m_pTiles[i]->SetEntity(pNewEntity);
	}

	EventMoveFinished::Get()->Register(this, &Grid::OnMoveComplete);

	Locator::GetAudio()->LoadSound("Assets/Sound/BellComplete.wav", &m_comboSound);

	Reset();
}

//-------------------------------------------------------------------------------
Grid::~Grid()
{
	for (int i=0; i< GRID_SIZE*GRID_SIZE; ++i)
	{
		Entity* pEntity = m_pTiles[i]->GetEntity();
		TSafeDelete(pEntity);

		TSafeDelete(m_pTiles[i]);
	}

	TSafeDelete(m_pDefinition);

	EventMoveFinished::Get()->Unregister(this, &Grid::OnMoveComplete);

	m_pMatchedTiles.clear();
	m_matchedColumns.clear();
}

//-------------------------------------------------------------------------------
const TVector2& Grid::GetOffset() const
{
	return m_offset;
}

//-------------------------------------------------------------------------------
int Grid::GetTileSize() const
{
	return m_nTileSize;
}

//-------------------------------------------------------------------------------
void Grid::Reset()
{
	m_lastSwap.pStart = NULL;
	m_lastSwap.pEnd = NULL;

	for (int i=0; i< GRID_SIZE*GRID_SIZE; ++i)
	{
		Entity* pEntity = m_pTiles[i]->GetEntity();

		TVector3 position;
		position.x = m_offset.x + m_pTiles[i]->GetX() * (float)m_nTileSize;
		position.y = m_offset.y + m_pTiles[i]->GetY() * (float)m_nTileSize;
		position.z = 0.f;

		pEntity->SetPosition(position);

		SpriteAssigner::Get()->AssignSprite(pEntity);

		MoverPlugin* pMover = static_cast<MoverPlugin*>(pEntity->GetPlugin(moverStr));
		pMover->Stop();

		BouncerPlugin* pBouncer = static_cast<BouncerPlugin*>(pEntity->GetPlugin("Bouncer"));
		pBouncer->Stop();
	}
}

//-------------------------------------------------------------------------------
GridTile* Grid::GetTile(const TVector2& screenPosition) const
{
	int x, y;

	ScreenToGrid(screenPosition, x, y);

	return GetTile(x, y);
}

//-------------------------------------------------------------------------------
void Grid::ScreenToGrid(const TVector2& screenPos, int& x, int& y) const
{
	x = (int)((screenPos.x - m_offset.x) / m_nTileSize);
	y = (int)((screenPos.y - m_offset.y) / m_nTileSize);
}

//-------------------------------------------------------------------------------
GridTile* Grid::GetTile(int x, int y) const
{
	if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE)
	{
		return NULL;
	}

	int index = (y * GRID_SIZE) + x;

	if (index >= 0 && index < GRID_SIZE*GRID_SIZE)
	{
		return m_pTiles[index];
	}

	return NULL;
}

//-------------------------------------------------------------------------------
void Grid::SwapTiles(GridTile* a, GridTile* b)
{
	if (a->IsAdjecent(*b))
	{
		Entity* temp = a->GetEntity();

		a->SetEntity(b->GetEntity());
		b->SetEntity(temp);

		MoverPlugin* pMover = static_cast<MoverPlugin*>(a->GetEntity()->GetPlugin(moverStr));
		pMover->SetPosition(a->GetEntity()->GetPosition(), b->GetEntity()->GetPosition(), m_fTransitionTime);

		pMover = static_cast<MoverPlugin*>(b->GetEntity()->GetPlugin(moverStr));
		pMover->SetPosition(b->GetEntity()->GetPosition(), a->GetEntity()->GetPosition(), m_fTransitionTime);

		m_lastSwap.pStart = a;
		m_lastSwap.pEnd = b;
	}
}

//-------------------------------------------------------------------------------
void Grid::OnMoveComplete()
{
	m_pMatchedTiles.clear();

	for (int i=0; i<GRID_SIZE * GRID_SIZE; ++i)
	{
		m_pTiles[i]->SetMatched(false);
	}

	if (CheckForCombo())
	{
		Locator::GetAudio()->PlaySound(m_comboSound);
		EventCombo::Get()->Execute();
	}

	// any matched tiles go to the top
	// other tiles in their column drop down
	for (unsigned int i=0; i<m_pMatchedTiles.size(); ++i)
	{
		// --- spawn smoke
		TVector2 smokePosition;
		smokePosition.x = m_pMatchedTiles[i]->GetEntity()->GetPosition().x;
		smokePosition.y = m_pMatchedTiles[i]->GetEntity()->GetPosition().y;

		TEffectManager::Get()->AddEffect(smokePosition, "Assets/2D/Smoke/Smoke.txt");

		// randomise the 'new' gem
		SpriteAssigner::Get()->AssignSprite(m_pMatchedTiles[i]->GetEntity());

		// move other tiles down
		m_matchedColumns.push_back(m_pMatchedTiles[i]->GetX());
	}

	for (unsigned int i=0; i<m_matchedColumns.size(); ++i)
	{
		TidyColumn(m_matchedColumns[i]);
	}

	m_matchedColumns.clear();
}

//-------------------------------------------------------------------------------
bool Grid::CheckForCombo()
{
	bool combo = false;

	for (int i=0; i<GRID_SIZE; ++i)
	{
		if (CheckLine(i, 0, 0, 1, m_nComboCount))
		{
			combo = true;
		}

		if (CheckLine(0, i, 1, 0, m_nComboCount))
		{
			combo = true;
		}
	}

	return combo;
}

//-------------------------------------------------------------------------------
bool Grid::CheckLine(int startX, int startY, int offsetX, int offsetY, int minCount)
{
	int x = startX;
	int y = startY;
	GridTile* pStart = GetTile(x, y);

	if (pStart == NULL)
	{
		return false;
	}

	x += offsetX;
	y += offsetY;
	GridTile* pNextTile = GetTile(x, y);

	int nRun = 1;

	while (pNextTile != NULL)
	{
		bool match = pNextTile->Matches(*pStart);

		if (match)
		{
			nRun++;
		}
		else
		{
			if (nRun >= minCount)
			{
				break;
			}

			pStart = pNextTile;
			nRun = 1;
		}

		x += offsetX;
		y += offsetY;
		pNextTile = GetTile(x, y);
	}

	if (nRun >= minCount)
	{
		ApplyCombo(pStart, nRun, offsetX, offsetY);

		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------
void Grid::ApplyCombo(GridTile* pStart, int count, int offsetX, int offsetY)
{
	int x = pStart->GetX() + offsetX;
	int y = pStart->GetY() + offsetY;

	AddMatchedTile(pStart);

	for (int i=1; i<count; ++i)
	{
		GridTile* pTile = GetTile(x, y);

		AddMatchedTile(pTile);

		x += offsetX;
		y += offsetY;
	}
}

//-------------------------------------------------------------------------------
void Grid::AddMatchedTile(GridTile* pTile)
{
	m_pMatchedTiles.push_back(pTile);
	pTile->SetMatched(true);

	Player::Get()->AddScore(m_nScorePerTile);
}

//-------------------------------------------------------------------------------
void Grid::TidyColumn(int column)
{
	GridTile* pStart = NULL;
	int y = GRID_SIZE - 1;
	GridTile* pTile = GetTile(column, y);

	// find the first matched tile from the bottom up
	while (pTile != NULL && !pTile->IsMatched())
	{
		y--;
		pTile = GetTile(column, y);
	}

	// mark it as the start
	if (pTile != NULL)
	{
		pStart = pTile;
	}

	// find an unmarked tile above it
	while (pTile != NULL && pTile->IsMatched())
	{
		y--;
		pTile = GetTile(column, y);
	}

	// swap them
	if (pTile != NULL)
	{
		Entity* temp = pTile->GetEntity();

		pTile->SetEntity(pStart->GetEntity());
		pStart->SetEntity(temp);

		TVector3 startDestination(
			m_offset.x + pStart->GetX() * m_nTileSize,
			m_offset.y + pStart->GetY() * m_nTileSize,
			0.f);

		TVector3 tileDestination(
			m_offset.x + pTile->GetX() * m_nTileSize,
			m_offset.y + pTile->GetY() * m_nTileSize,
			0.f);

		MoverPlugin* pMover = static_cast<MoverPlugin*>(pTile->GetEntity()->GetPlugin(moverStr));
		pMover->SetPosition(pTile->GetEntity()->GetPosition(), tileDestination, 1.0f);

		pMover = static_cast<MoverPlugin*>(pStart->GetEntity()->GetPlugin(moverStr));
		pMover->SetPosition(pStart->GetEntity()->GetPosition(), startDestination, 1.0f);

		pTile->SetMatched(true);
		pStart->SetMatched(false);

		TidyColumn(column);
	}
	else
	{
		// all unmatched tiles have been moved down, now fill the open slots
		for (int i=0; i<GRID_SIZE; ++i)
		{
			pTile = GetTile(column, i);

			if (pTile != NULL && pTile->IsMatched())
			{
				TVector3 destination;
				destination.x = m_offset.x + pTile->GetX() * m_nTileSize;
				destination.y = m_offset.y + pTile->GetY() * m_nTileSize;
				destination.z = 0.f;

				float y = (float)(m_nTileSize + pTile->GetY()) * (m_nTileSize / 2);
				TVector3 offset(0, y, 0);

				MoverPlugin* pMover = static_cast<MoverPlugin*>(pTile->GetEntity()->GetPlugin(moverStr));
				pMover->SetPosition(pTile->GetEntity()->GetPosition() - offset, destination, 1.0f);
			}
		}
	}
}
