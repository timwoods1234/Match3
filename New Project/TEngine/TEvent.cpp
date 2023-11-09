#include "TEvent.h"

#include "TEventListener.h"
#include "TEventRender.h"
#include "TEventUpdate.h"
#include "TEventRender2D.h"
#include "TEventRender3D.h"

//-------------------------------------------------------------------------------
TEvent::TEvent()
{
}

//-------------------------------------------------------------------------------
TEvent::~TEvent()
{
	Clear();
}

//-------------------------------------------------------------------------------
void TEvent::Execute()
{
	for (unsigned int i=0; i<m_pListeners.size(); ++i)
	{
		m_pListeners[i]->Execute();
	}
}
