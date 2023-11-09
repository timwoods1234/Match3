#include "TError.h"
#include "DataLoader.h"
#include "SpritePluginDefinition.h"
#include "SpritePlugin.h"

//-------------------------------------------------------------------------------
RegisterPluginDefinition(SpritePluginDefinition, SpritePlugin);

//-------------------------------------------------------------------------------
SpritePluginDefinition::SpritePluginDefinition()
{
	m_strName = "Sprite";
}

//-------------------------------------------------------------------------------
void SpritePluginDefinition::Init(DataLoader* pLoader)
{
	if (pLoader == NULL || !pLoader->GetString("Graphic", m_strFilename))
	{
		TDebugPrint(("[SpritePluginDefinition::Init] Could not find sprite filename"));
	}
}

//-------------------------------------------------------------------------------
const TString& SpritePluginDefinition::GetFilename() const
{
	return m_strFilename;
}
