#ifndef __TEVENTRENDER3D_H__
#define __TEVENTRENDER3D_H__

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef __TEVENT_H__
#include "TEvent.h"
#endif

#ifndef __TERROR_H__
#include "TError.h"
#endif

class TEventRender3D : public TEvent,
					   public Singleton<TEventRender3D>
{
public:
	//-------------------------------------------------------------------------------
	TEventRender3D()
	{
	}

	//-------------------------------------------------------------------------------
	~TEventRender3D()
	{
		TASSERT( (m_pListeners.size() == 0), "[TEventRender3D] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(TEventRender3D);

#endif
