#ifndef __BOUNCERPLUGIN_H__
#define __BOUNCERPLUGIN_H__

//+-------------------------------------------------------
//| Controls a subtle bounce when an object finishes moving
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

#ifndef __BOUNCERPLUGINDEFINITION_H__
#include "BouncerPluginDefinition.h"
#endif

class SpritePlugin;

class BouncerPlugin : public Plugin,
					public TEventListener
{
public:
	BouncerPlugin(Entity* pEntity, BouncerPluginDefinition* pDefinition);
	~BouncerPlugin();

	void Begin(float fDuration);
	void OnUpdate(float elapsedTime);
	void Stop();

private:
	TVector2 m_offset;
	float m_fTimer;
	SpritePlugin* m_pSprite;
};

#endif
