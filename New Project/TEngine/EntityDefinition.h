#ifndef __ENTITYDEFINITION_H__
#define __ENTITYDEFINITION_H__

//+-------------------------------------------------------
//| Container for plugin definitions
//| Version 1.000
//+-------------------------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#include <vector>

class PluginDefinition;
class Entity;

class EntityDefinition
{
public:
	EntityDefinition();
	~EntityDefinition();

	void AddPluginDefinition(PluginDefinition* pDefinition);
	bool HasPlugin(const TString& pluginName) const;
	void SetFilename(const TString& filename);
	PluginDefinition* GetPluginDefinition(const TString& name) const;
	
	// for each plugin definition, attach a new instance to the entity
	void CreatePlugins(Entity* pEntity);
private:
	TString m_strFilename;
	std::vector<class PluginDefinition*> m_pPluginDefinitions;
};

#endif