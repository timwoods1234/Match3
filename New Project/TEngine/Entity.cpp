#include "TError.h"
#include "TMemory.h"

#include "Entity.h"
#include "EntityDefinition.h"
#include "Plugin.h"
#include "PluginDefinition.h"

//-------------------------------------------------------------------------------
Entity::Entity()
:	m_pDefinition(NULL),
	m_position(0,0,0)
{
}

//-------------------------------------------------------------------------------
Entity::~Entity()
{
	for (unsigned int i=0; i<m_pPlugins.size(); ++i)
	{
		TSafeDelete(m_pPlugins[i]);
	}

	m_pPlugins.clear();

	m_pDefinition = NULL;
}

//-------------------------------------------------------------------------------
Entity::Entity(EntityDefinition *pDefinition)
:	m_pDefinition(pDefinition),
	m_position(0,0,0)
{
	if (pDefinition != NULL)
	{
		pDefinition->CreatePlugins(this);
	}
}

//-------------------------------------------------------------------------------
Plugin* Entity::AddPlugin(const TString &name, PluginDefinition *pDefinition)
{
	Plugin* result = NULL;

	// does it already exist?
	result = GetPlugin(name);

	if (result != NULL)
	{
		return result;
	}

	if (pDefinition != NULL)
	{
		result = pDefinition->CreatePlugin(this);
	}
	else
	{
		// see if our definition can be used
		pDefinition = m_pDefinition->GetPluginDefinition(name);

		if (pDefinition != NULL)
		{
			result = pDefinition->CreatePlugin(this);
		}
		else
		{
			TDebugPrint(("[Entity::AddPlugin] There is no definition for %s", *name));
		}
	}

	if (result != NULL)
	{
		m_pPlugins.push_back(result);
	}

	return result;
}

//-------------------------------------------------------------------------------
Plugin* Entity::GetPlugin(const TString &strName) const
{
	for (unsigned int i=0; i<m_pPlugins.size(); ++i)
	{
		if (strName == m_pPlugins[i]->GetDefinition()->GetName())
		{
			return m_pPlugins[i];
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------------
const TVector3& Entity::GetPosition() const
{
	return m_position;
}

//-------------------------------------------------------------------------------
void Entity::SetPosition(const TVector3& position)
{
	m_position = position;
}
