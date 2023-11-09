#ifndef __TEFFECTMANAGER_H__
#define __TEFFECTMANAGER_H__

//+----------------------------------------
//| Manager class for billboarded effects.
//| Version 1.000
//+----------------------------------------

#include <vector>

#ifndef _SINGLETON_H__
#include "Singleton.h"
#endif

#ifndef __TVECTOR3_H__
#include "TVector3.h"
#endif

#ifndef __TEVENTLISTENER_H__
#include "TEventListener.h"
#endif

class TAnimatedSprite;
class TMatrix;

class TEffectManager : public TEventListener,
					   public Singleton<TEffectManager>
{
public:
	TEffectManager();
	~TEffectManager();

	void Reset();
	bool AddEffect(const TVector2& position, const char* filename);
	void OnUpdate(float elapsedTime);
	void OnRender();

	// pre-load files to make sure they're in memory
	bool PreloadFromFile(const char* filename);

private:
	// TODO: wrap this inside a struct
	std::vector<TAnimatedSprite*>	m_effects;
	std::vector<TVector2>			m_effectPositions;
};

#endif
