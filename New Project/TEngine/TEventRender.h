#ifndef __TEVENTRENDER_H__
#define __TEVENTRENDER_H__

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef __TEVENT_H__
#include "TEvent.h"
#endif

#ifndef __TERROR_H__
#include "TError.h"
#endif

class TEventRender : public TEvent,
					 public Singleton<TEventRender>
{
public:
	//-------------------------------------------------------------------------------
	TEventRender()
	{
	}

	//-------------------------------------------------------------------------------
	~TEventRender()
	{
		TASSERT( (m_pListeners.size() == 0), "[TEventRender] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(TEventRender);

#endif
