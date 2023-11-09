#ifndef __SPRITEPLUGINDEFINITION_H__
#define __SPRITEPLUGINDEFINITION_H__

//+-------------------------------------------------------
//| Definition for a sprite plugin
//+-------------------------------------------------------

#ifndef __PLUGINDEFINITIN_H__
#include "PluginDefinition.h"
#endif

class SpritePluginDefinition : public PluginDefinition
{
public:
	SpritePluginDefinition();
	void Init(DataLoader* pLoader);
	Plugin* CreatePlugin(Entity* pEntity);

	const TString& GetFilename() const;

private:
	TString m_strFilename;
};

#endif
