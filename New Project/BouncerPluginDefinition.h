#ifndef __BOUNCERPLUGINDEFINITION_H__
#define __BOUNCERPLUGINDEFINITION_H__

//+-------------------------------------------------------
//| Definition for a bouncer plugin
//| If anything was read from tile this would store it
//+-------------------------------------------------------

#ifndef __PLUGINDEFINITIN_H__
#include "PluginDefinition.h"
#endif

class BouncerPluginDefinition : public PluginDefinition
{
public:
	BouncerPluginDefinition();
	void Init(DataLoader* pLoader);
	Plugin* CreatePlugin(Entity* pEntity);
};

#endif
