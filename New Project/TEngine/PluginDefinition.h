#ifndef __PLUGINDEFINITIN_H__
#define __PLUGINDEFINITIN_H__

//+-------------------------------------------------------
//| Holds data for a single type of plugin.
//| Version 1.000
//+-------------------------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __TMEMORY_H__
#include "TMemory.h"
#endif

class Entity;
class Plugin;
class DataLoader;

class PluginDefinition
{
public:
	virtual ~PluginDefinition() {}

	// allows the plugin to read data from file
	virtual void Init(DataLoader* pLoader) {}

	// attach a new instance to an entity
	virtual Plugin* CreatePlugin(Entity* pEntity) = 0;

	const TString& GetName() const;
	
protected:
	TString m_strName;
};

//-------------------------------------------------------------------------------
#define RegisterPluginDefinition(PluginDefinitionType, PluginType) \
	Plugin* PluginDefinitionType::CreatePlugin(Entity* pEntity) \
	{ \
		PluginType* instance = TNew PluginType(pEntity, this); \
		return instance; \
	}

#endif