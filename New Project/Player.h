#ifndef __PLAYER_H__
#define __PLAYER_H__

//+-------------------------------------------------------
//| Stores Player data (score)
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class Player : public Singleton<Player>
{
public:
	Player();
	~Player();

	void Reset();
	void AddScore(int points);
	int GetScore() const;
	int GetContinueCost() const;
	void SetContinueCost(int cost);

private:
	int m_nScore;
	int m_nContinueCost;
};

DefineSingleton(Player);

#endif
