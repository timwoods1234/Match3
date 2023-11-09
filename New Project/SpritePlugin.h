#ifndef __SPRITEPLUGIN_H__
#define __SPRITEPLUGIN_H__

//+-------------------------------------------------------
//| Plugin for a 2D sprite (not animated)
//+-------------------------------------------------------


#ifndef __PLUGIN_H__
#include "Plugin.h"
#endif

#ifndef __SPRITEPLUGINDEFINITION_H__
#include "SpritePluginDefinition.h"
#endif

#ifndef __TSPRITE_H__
#include "TSprite.h"
#endif

#ifndef __TEVENTLISTENER_H__
#include "TEventListener.h"
#endif

class SpritePlugin : public Plugin,
					 public TEventListener
{
public:
	SpritePlugin(Entity* pEntity, SpritePluginDefinition* pDefinition);
	~SpritePlugin();

	const TVector3& GetPosition() const;

	void SetPosition(const TVector3& pos);

	void OnRender();

	void LoadTexture(const char* filename);

	TSprite* GetSprite();

	const TVector2& GetRenderOffset() const;

	void SetRenderOffset(const TVector2& offset);

private:
	TSprite m_sprite;
	TVector2 m_renderOffset;
};

#endif
