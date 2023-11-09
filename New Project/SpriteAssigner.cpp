#include "SpritePlugin.h"
#include "SpriteAssigner.h"

//-------------------------------------------------------------------------------
SpriteAssigner::SpriteAssigner()
{
	m_strFilenames.push_back("Assets/2D/Blue.png");
	m_strFilenames.push_back("Assets/2D/Green.png");
	m_strFilenames.push_back("Assets/2D/Red.png");
	m_strFilenames.push_back("Assets/2D/Purple.png");
	m_strFilenames.push_back("Assets/2D/Yellow.png");
}

//-------------------------------------------------------------------------------
void SpriteAssigner::AssignSprite(Entity* pEntity)
{
	static const TString sprite = "Sprite";
	static unsigned int index = 0;

	index++;
	if (index >= m_strFilenames.size()) index= 0;

	SpritePlugin* pSprite = static_cast<SpritePlugin*>(pEntity->GetPlugin(sprite));

	pSprite->GetSprite()->LoadTexture(*(m_strFilenames[index]));
}
