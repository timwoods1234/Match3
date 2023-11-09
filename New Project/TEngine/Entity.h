#ifndef __ENTITY_H__
#define __ENTITY_H__

//+-------------------------------------------------------
//| Container for plugin instances
//| Version 1.000
//+-------------------------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __TVECTOR3_H__
#include "TVector3.h"
#endif

#include <vector>

class EntityDefinition;
class Plugin;
class PluginDefinition;

class Entity
{
public:
	Entity();

	Entity(EntityDefinition* pDefinition);

	~Entity();

	Plugin* AddPlugin(const TString& name, PluginDefinition* pDefinition);

	Plugin* GetPlugin(const TString& strName) const;

	const TVector3& GetPosition() const;

	void SetPosition(const TVector3& position);
private:
	EntityDefinition* m_pDefinition;
	std::vector<Plugin*> m_pPlugins;
	TVector3 m_position;
};

#endif