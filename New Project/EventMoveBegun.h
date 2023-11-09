#ifndef __EVENTMOVEBEGUN_H__
#define __EVENTMOVEBEGUN_H__

//+-------------------------------------------------------
//| Event for when an object starts moving
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

#ifndef __TEVENT_H__
#include "TEvent.h"
#endif

class MoverPlugin;

class EventMoveBegun : public TEvent, public Singleton<EventMoveBegun>
{
public:
	//-------------------------------------------------------------------------------
	EventMoveBegun()
	{
	}

	//-------------------------------------------------------------------------------
	~EventMoveBegun()
	{
		TASSERT( (m_pListeners.size() == 0), "[EventMoveBegun] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(EventMoveBegun);

#endif
