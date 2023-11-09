#include "TEventUpdate.h"

#include "EventMoveFinished.h"
#include "EventMoveBegun.h"
#include "MoverManager.h"

//-------------------------------------------------------------------------------
MoverManager::MoverManager()
:	m_bTriggerBegin(false),
	m_bTriggerEnd(false)
{
	TEventUpdate::Get()->Register(this, &MoverManager::OnUpdate);
}

//-------------------------------------------------------------------------------
MoverManager::~MoverManager()
{
	TEventUpdate::Get()->Unregister(this, &MoverManager::OnUpdate);
}

//-------------------------------------------------------------------------------
void MoverManager::OnUpdate(float elapsedTime)
{
	if (m_bTriggerBegin)
	{
		m_bTriggerBegin = false;

		EventMoveBegun::Get()->Execute();
	}

	if (m_bTriggerEnd)
	{
		m_bTriggerEnd = false;

		EventMoveFinished::Get()->Execute();
	}
}

//-------------------------------------------------------------------------------
void MoverManager::RequestTriggerBegin()
{
	m_bTriggerBegin = true;
}

//-------------------------------------------------------------------------------
void MoverManager::RequestTriggerEnd()
{
	m_bTriggerEnd = true;
}
