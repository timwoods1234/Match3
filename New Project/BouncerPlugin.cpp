#include "BouncerPlugin.h"
#include "Entity.h"
#include "TEventUpdate.h"
#include "EventMoveFinished.h"
#include "EventMoveBegun.h"
#include "SpritePlugin.h"

//-------------------------------------------------------------------------------
BouncerPlugin::BouncerPlugin(Entity* pEntity, BouncerPluginDefinition* pDefinition)
:	Plugin(pEntity, pDefinition),
	m_fTimer(0.f)
{
	TEventUpdate::Get()->Register(this, &BouncerPlugin::OnUpdate);

	static const TString strSprite = "Sprite";
	m_pSprite = static_cast<SpritePlugin*>(pEntity->AddPlugin(strSprite, NULL));
}

//-------------------------------------------------------------------------------
BouncerPlugin::~BouncerPlugin()
{
	TEventUpdate::Get()->Unregister(this, &BouncerPlugin::OnUpdate);
}

//-------------------------------------------------------------------------------
void BouncerPlugin::Begin(float fDuration)
{
	m_fTimer = fDuration;

	m_offset.x = 0.f;
	m_offset.y = 0.f;
}

//-------------------------------------------------------------------------------
void BouncerPlugin::Stop()
{
	m_fTimer = 0.f;
}

//-------------------------------------------------------------------------------
void BouncerPlugin::OnUpdate(float elapsedTime)
{
	if (m_fTimer > 0.f)
	{
		m_fTimer -= elapsedTime;

		m_offset.y = 2.5f - sin(m_fTimer * 10.f);

		m_pSprite->SetRenderOffset(m_offset);
	}
}
