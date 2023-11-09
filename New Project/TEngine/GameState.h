#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

//+-------------------------------------------------------
//| Base class for game states
//| Version 1.000
//+-------------------------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

class GameState
{
public:
	GameState() {};
	virtual ~GameState() {};

	virtual void OnEntry(){};
	virtual void OnExit(){};
	virtual void OnLostDevice(){};
	virtual void OnResetDevice(){};

	const TString& GetStateName() const
	{
		return m_strStateName;
	};

protected:
	TString m_strStateName;
};

#endif