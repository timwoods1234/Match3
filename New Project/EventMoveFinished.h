#ifndef __EVENTMOVEFINISHED_H__
#define __EVENTMOVEFINISHED_H__

//+-------------------------------------------------------
//| Event for when an object has finished moving
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

#ifndef __TEVENT_H__
#include "TEvent.h"
#endif

class MoverPlugin;

class EventMoveFinished : public TEvent, public Singleton<EventMoveFinished>
{
public:
	//-------------------------------------------------------------------------------
	EventMoveFinished()
	{
	}

	//-------------------------------------------------------------------------------
	~EventMoveFinished()
	{
		TASSERT( (m_pListeners.size() == 0), "[EventMoveFinished] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(EventMoveFinished);

#endif
