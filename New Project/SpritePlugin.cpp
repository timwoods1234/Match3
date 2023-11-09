#include "SpritePlugin.h"
#include "Entity.h"
#include "TEventRender2D.h"

//-------------------------------------------------------------------------------
SpritePlugin::SpritePlugin(Entity* pEntity, SpritePluginDefinition* pDefinition)
:	Plugin(pEntity, pDefinition),
	m_renderOffset(0,0)
{
	if (pDefinition != NULL && !pDefinition->GetFilename().IsEmpty())
	{
		m_sprite.LoadTexture(*pDefinition->GetFilename());
	}

	TEventRender2D::Get()->Register(this, &SpritePlugin::OnRender);
}

//-------------------------------------------------------------------------------
SpritePlugin::~SpritePlugin()
{
	TEventRender2D::Get()->Unregister(this, &SpritePlugin::OnRender);
}

//-------------------------------------------------------------------------------
void SpritePlugin::OnRender()
{
	static TVector2 renderPosition;

	renderPosition.x = m_pEntity->GetPosition().x + m_renderOffset.x;
	renderPosition.y = m_pEntity->GetPosition().y + m_renderOffset.y;

	m_sprite.Render(renderPosition);
}

//-------------------------------------------------------------------------------
void SpritePlugin::LoadTexture(const char* filename)
{
	m_sprite.LoadTexture(filename);
}

//-------------------------------------------------------------------------------
TSprite* SpritePlugin::GetSprite()
{
	return &m_sprite;
}

//-------------------------------------------------------------------------------
const TVector2& SpritePlugin::GetRenderOffset() const
{
	return m_renderOffset;
}

//-------------------------------------------------------------------------------
void SpritePlugin::SetRenderOffset(const TVector2& offset)
{
	m_renderOffset = offset;
}
