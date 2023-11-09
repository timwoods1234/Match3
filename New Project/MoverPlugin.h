#ifndef __MOVERPLUGIN_H__
#define __MOVERPLUGIN_H__

//+-------------------------------------------------------
//| Controls entity movement between two points
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

#ifndef __MOVERPLUGINDEFINITION_H__
#include "MoverPluginDefinition.h"
#endif

class MoverPlugin : public Plugin,
					public TEventListener
{
public:
	MoverPlugin(Entity* pEntity, MoverPluginDefinition* pDefinition);
	~MoverPlugin();

	void OnUpdate(float elapsedTime);
	void SetPosition(const TVector3& start, const TVector3& end, float fTime);
	void Stop();

private:
	TVector3 m_start;
	TVector3 m_destination;
	bool m_bInterpolating;
	float m_fTravelTime;

	float m_fTimer;
};

#endif
