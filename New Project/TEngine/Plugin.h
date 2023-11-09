#ifndef __PLUGIN_H__
#define __PLUGIN_H__

//+-------------------------------------------------------
//| Instance of a plugin.
//| Use this to model behaviour.
//| Version 1.000
//+-------------------------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

class Entity;
class PluginDefinition;

class Plugin
{
public:
	Plugin(Entity* pEntity, PluginDefinition* pDefinition);

	virtual ~Plugin();

	Entity* GetEntity() const;
	PluginDefinition* GetDefinition() const;

protected:
	PluginDefinition* m_pDefinition;
	Entity* m_pEntity;
};

#endif