#include "TMemory.h"

#include "EntityDefinition.h"
#include "Entity.h"
#include "PluginDefinition.h"

//-------------------------------------------------------------------------------
EntityDefinition::EntityDefinition()
{
	m_strFilename = "";
}

//-------------------------------------------------------------------------------
EntityDefinition::~EntityDefinition()
{
	for (unsigned int i=0; i<m_pPluginDefinitions.size(); ++i)
	{
		TSafeDelete(m_pPluginDefinitions[i]);
	}

	m_pPluginDefinitions.clear();
}

//-------------------------------------------------------------------------------
void EntityDefinition::AddPluginDefinition(PluginDefinition* pDefinition)
{
	m_pPluginDefinitions.push_back(pDefinition);
}

//-------------------------------------------------------------------------------
bool EntityDefinition::HasPlugin(const TString& pluginName) const
{
	for (unsigned int i=0; i<m_pPluginDefinitions.size(); ++i)
	{
		if (pluginName == m_pPluginDefinitions[i]->GetName())
		{
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------------
void EntityDefinition::SetFilename(const TString& filename)
{
	m_strFilename = filename;
}

//-------------------------------------------------------------------------------
PluginDefinition* EntityDefinition::GetPluginDefinition(const TString& name) const
{
	for (unsigned int i=0; i<m_pPluginDefinitions.size(); ++i)
	{
		if (m_pPluginDefinitions[i]->GetName() == name)
		{
			return m_pPluginDefinitions[i];
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------------
void EntityDefinition::CreatePlugins(Entity* pEntity)
{
	for (unsigned int i=0; i<m_pPluginDefinitions.size(); ++i)
	{
		pEntity->AddPlugin(m_pPluginDefinitions[i]->GetName(), m_pPluginDefinitions[i]);
	}
}
