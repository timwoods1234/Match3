#include "Plugin.h"

//-------------------------------------------------------------------------------
Plugin::Plugin(Entity* pEntity, PluginDefinition* pDefinition)
{
	m_pDefinition = pDefinition;
	m_pEntity = pEntity;
}

//-------------------------------------------------------------------------------
Plugin::~Plugin()
{
	m_pEntity = NULL;
	m_pDefinition = NULL;
}

//-------------------------------------------------------------------------------
PluginDefinition* Plugin::GetDefinition() const
{
	return m_pDefinition;
}

//-------------------------------------------------------------------------------
Entity* Plugin::GetEntity() const
{
	return m_pEntity;
}
