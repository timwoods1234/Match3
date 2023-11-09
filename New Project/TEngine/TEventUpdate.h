#ifndef __TEVENTUPDATE_H__
#define __TEVENTUPDATE_H__

#ifndef _SINGLETON_H_
#include "Singleton.h"
#endif

#ifndef __TEVENTPARAM_H__
#include "TEventParam.h"
#endif

#ifndef __TERROR_H__
#include "TError.h"
#endif

class TEventUpdate : public TEventParam<float>, public Singleton<TEventUpdate>
{
public:
	//-------------------------------------------------------------------------------
	TEventUpdate()
	{
	}

	//-------------------------------------------------------------------------------
	~TEventUpdate()
	{
		TASSERT( (m_pListeners.size() == 0), "[TEventUpdate] Event still has listeners!");
	}
};

//-------------------------------------------------------------------------------
DefineSingleton(TEventUpdate);

#endif
