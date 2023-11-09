#ifndef __SPRITEASSIGNER_H__
#define __SPRITEASSIGNER_H__

//+-------------------------------------------------------
//| Wraps a function to assign a sprite to an entity
//+-------------------------------------------------------

#ifndef __TENGINEINCLUDED_H__
#include "TEngine.h"
#endif

class SpriteAssigner : public Singleton<SpriteAssigner>
{
public:
	SpriteAssigner();

	void AssignSprite(Entity* pEntity);
	
private:
	std::vector<TString> m_strFilenames;
};

DefineSingleton(SpriteAssigner);

#endif