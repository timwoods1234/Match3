#include "TError.h"
#include "DataLoader.h"
#include "BouncerPluginDefinition.h"
#include "BouncerPlugin.h"

//-------------------------------------------------------------------------------
RegisterPluginDefinition(BouncerPluginDefinition, BouncerPlugin);

//-------------------------------------------------------------------------------
BouncerPluginDefinition::BouncerPluginDefinition()
{
	m_strName = "Bouncer";
}

//-------------------------------------------------------------------------------
void BouncerPluginDefinition::Init(DataLoader* pLoader)
{
}
