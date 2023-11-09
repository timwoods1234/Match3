#ifndef __MOVERMANAGER_H__
#define __MOVERMANAGER_H__

//+-------------------------------------------------------
//| Manages events for movers (begin and end)
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class MoverManager : public Singleton<MoverManager>,
					 public TEventListener
{
public:
	MoverManager();
	~MoverManager();

	void OnUpdate(float elapsedTime);
	void RequestTriggerBegin();
	void RequestTriggerEnd();

private:
	bool m_bTriggerBegin;
	bool m_bTriggerEnd;
};

DefineSingleton(MoverManager);

#endif
