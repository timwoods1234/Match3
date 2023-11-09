#include "TError.h"
#include "DataLoader.h"
#include "MoverPluginDefinition.h"
#include "MoverPlugin.h"

//-------------------------------------------------------------------------------
RegisterPluginDefinition(MoverPluginDefinition, MoverPlugin);

//-------------------------------------------------------------------------------
MoverPluginDefinition::MoverPluginDefinition()
{
	m_strName = "Mover";
}

//-------------------------------------------------------------------------------
void MoverPluginDefinition::Init(DataLoader* pLoader)
{
}
