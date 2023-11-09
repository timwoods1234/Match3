#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

//+-------------------------------------------------------
//| Container for GameState objects
//| Version 1.002
//+-------------------------------------------------------

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

class GameState;

class GameStateManager : public Singleton<GameStateManager>
{
public:
	GameStateManager();
	~GameStateManager();

	void AddState(GameState* state);
	bool SwitchState(const char* stateName);
	const char* GetCurrentStateName();
	void Clear();

	void OnLostDevice();
	void OnResetDevice();

private:
	GameState* m_currentState;
	std::vector<GameState*> m_stateList;
};

DefineSingleton(GameStateManager);


#endif
