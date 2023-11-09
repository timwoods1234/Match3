#ifndef __TEVENT_H__
#define __TEVENT_H__

//+----------------------------------------
//| Simple event that has no params.
//| Version 1.000
//+----------------------------------------

#include <vector>

#ifndef __TEVENTHANDLER_H__
#include "TEventHandler.h"
#endif

#ifndef __TMEMORY_H__
#include "TMemory.h"
#endif

class TEvent
{
public:
	TEvent();
	virtual ~TEvent();

	virtual void Execute();		// send the event to all listeners

	template <class ObjectType>
	inline void		Register(
						ObjectType* object,
						typename TEventHandler<ObjectType>::Handler handler
						);

	template <class ObjectType>
	inline void		Unregister(
						ObjectType* object,
						typename TEventHandler<ObjectType>::Handler handler
						);
protected:
	std::vector<TEventHandlerBase*> m_pListeners;

	inline void Clear();
};

#include "TEvent.inl"

#endif //#ifndef __TEVENT_H__
