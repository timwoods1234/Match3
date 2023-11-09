#include "GameState.h"
#include "GameStateManager.h"
#include "TMemory.h"
#include "TError.h"

//-------------------------------------------------------------------------------
GameStateManager::GameStateManager()
{
	m_currentState = 0;
}

//-------------------------------------------------------------------------------
GameStateManager::~GameStateManager()
{
	m_currentState->OnExit();

	for (int i=0; i < (int)m_stateList.size(); i++)
	{
		TSafeDelete(m_stateList[i]);
	}

	Clear();
}

//-------------------------------------------------------------------------------
void GameStateManager::AddState(GameState *state)
{
	for (int i = 0; i < (int)m_stateList.size(); i++)
	{
		if(m_stateList.at(i)->GetStateName() == state->GetStateName())
		{
			//state already exists
			TDebugPrint(("[GameStateManager::AddState] Attempted to add state twice (%s)", *state->GetStateName() ));
			return;
		}
	}
	m_stateList.push_back(state);
}

//-------------------------------------------------------------------------------
bool GameStateManager::SwitchState(const char* stateName)
{
	for (int i = 0; i < (int)m_stateList.size(); i++)
	{
		if(m_stateList.at(i)->GetStateName() == stateName)
		{
			if(m_currentState)
			{
				m_currentState->OnExit();
			}

			m_currentState = m_stateList.at(i);
			m_currentState->OnEntry();

			return true;
		}
	}

	TDebugPrint(("[GameStateManager::SwitchState] Couldn't find state '%s'", stateName));
	return false;
}

//-------------------------------------------------------------------------------
const char* GameStateManager::GetCurrentStateName()
{
	return *(m_currentState->GetStateName());
}

//-------------------------------------------------------------------------------
void GameStateManager::Clear()
{
	m_stateList.clear();
}

//-------------------------------------------------------------------------------
void GameStateManager::OnLostDevice()
{
	for (int i = 0; i < (int)m_stateList.size(); i++)
	{
		m_stateList.at(i)->OnLostDevice();
	}
}

//-------------------------------------------------------------------------------
void GameStateManager::OnResetDevice()
{
	for (int i = 0; i < (int)m_stateList.size(); i++)
	{
		m_stateList.at(i)->OnResetDevice();
	}
}
