#include "MoverPlugin.h"
#include "Entity.h"
#include "TEventUpdate.h"
#include "BouncerPlugin.h"
#include "MoverManager.h"

//-------------------------------------------------------------------------------
MoverPlugin::MoverPlugin(Entity* pEntity, MoverPluginDefinition* pDefinition)
:	Plugin(pEntity, pDefinition),
	m_bInterpolating(false),
	m_fTimer(0.f)
{
	TEventUpdate::Get()->Register(this, &MoverPlugin::OnUpdate);
}

//-------------------------------------------------------------------------------
MoverPlugin::~MoverPlugin()
{
	TEventUpdate::Get()->Unregister(this, &MoverPlugin::OnUpdate);
}

//-------------------------------------------------------------------------------
void MoverPlugin::SetPosition(const TVector3& start, const TVector3& end, float fTime)
{
	m_bInterpolating = true;
	m_start = start;
	m_destination = end;
	m_fTravelTime = fTime;

	m_fTimer = 0.f;

	MoverManager::Get()->RequestTriggerBegin();
}

//-------------------------------------------------------------------------------
void MoverPlugin::Stop()
{
	m_bInterpolating = false;
}

//-------------------------------------------------------------------------------
void MoverPlugin::OnUpdate(float elapsedTime)
{
	if (m_bInterpolating)
	{
		m_fTimer += elapsedTime;

		float fraction = TMin( (m_fTimer / m_fTravelTime), 1.f);

		TVector3 position = TLerp(m_start, m_destination, fraction);

		m_pEntity->SetPosition(position);

		if (fraction == 1.f)
		{
			m_bInterpolating = false;
			
			MoverManager::Get()->RequestTriggerEnd();

			static const TString strBouncer = "Bouncer";
			BouncerPlugin* pBouncer = static_cast<BouncerPlugin*>(m_pEntity->GetPlugin(strBouncer));

			pBouncer->Begin(1.f);
		}
	}
}
