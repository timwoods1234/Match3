#ifndef __PLUGINFACTORY_H__
#define __PLUGINFACTORY_H__

#include "SpritePluginDefinition.h"
#include "MoverPluginDefinition.h"
#include "BouncerPluginDefinition.h"

//-------------------------------------------------------------------------------
#define CreatePluginDefinition(type, definition) \
	type* type##instance = TNew type(); \
	definition->AddPluginDefinition(type##instance);

//-------------------------------------------------------------------------------
static EntityDefinition* CreateDefinition()
{
	// typically, a definition is read from a file
	// and plugins are attached based on what the file contains
	// but for this game, entities are all the same
	// so I just add the plugin definitions I want here
	EntityDefinition* pDefinition = TNew EntityDefinition();

	CreatePluginDefinition(SpritePluginDefinition, pDefinition);
	CreatePluginDefinition(MoverPluginDefinition, pDefinition);
	CreatePluginDefinition(BouncerPluginDefinition, pDefinition);

	return pDefinition;
}

// ---------------------------------------------------------------------
static Entity* CreateEntity(EntityDefinition* pDef)
{
	Entity* e = TNew Entity(pDef);	// will add plugins from the definition
	
	return e;
}

#endif
