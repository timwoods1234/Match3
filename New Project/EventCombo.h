#ifndef __EVENTCOMBO_H__
#define __EVENTCOMBO_H__

//+-------------------------------------------------------
//| Event for when a combo is scored
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

#ifndef __TEVENT_H__
#include "TEvent.h"
#endif

class EventCombo : public TEvent, public Singleton<EventCombo>
{
public:
	//-------------------------------------------------------------------------------
	EventCombo()
	{
	}

	//-------------------------------------------------------------------------------
	~EventCombo()
	{
		TASSERT( (m_pListeners.size() == 0), "[EventCombo] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(EventCombo);

#endif
