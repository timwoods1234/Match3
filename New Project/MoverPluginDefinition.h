#ifndef __MOVERPLUGINDEFINITION_H__
#define __MOVERPLUGINDEFINITION_H__

//+-------------------------------------------------------
//| Definition for a mover plugin
//| If anything was read from tile this would store it
//+-------------------------------------------------------

#ifndef __PLUGINDEFINITIN_H__
#include "PluginDefinition.h"
#endif

class MoverPluginDefinition : public PluginDefinition
{
public:
	MoverPluginDefinition();
	void Init(DataLoader* pLoader);
	Plugin* CreatePlugin(Entity* pEntity);
};

#endif
