#ifndef __TEVENTRENDER2D_H__
#define __TEVENTRENDER2D_H__

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef __TEVENT_H__
#include "TEvent.h"
#endif

#ifndef __TERROR_H__
#include "TError.h"
#endif

class TEventRender2D : public TEvent,
					   public Singleton<TEventRender2D>
{
public:
	//-------------------------------------------------------------------------------
	TEventRender2D()
	{
	}

	//-------------------------------------------------------------------------------
	~TEventRender2D()
	{
		TASSERT( (m_pListeners.size() == 0), "[TEventRender2D] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(TEventRender2D);

#endif
