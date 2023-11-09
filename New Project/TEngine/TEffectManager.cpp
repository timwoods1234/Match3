#include "TAnimatedSprite.h"
#include "SafeRelease.h"
#include "Locator.h"
#include "IRenderer.h"
#include "Camera.h"
#include "TMemory.h"
#include "TEventUpdate.h"
#include "TEventRender2D.h"
#include "EngineGlobals.h"

#include "TEffectManager.h"

//-------------------------------------------------------------------------------
DefineSingleton(TEffectManager);

//-------------------------------------------------------------------------------
TEffectManager::TEffectManager()
{
	TEventUpdate::Get()->Register(this, &TEffectManager::OnUpdate);
	TEventRender2D::Get()->Register(this, &TEffectManager::OnRender);
}

//-------------------------------------------------------------------------------
TEffectManager::~TEffectManager()
{
	TEventUpdate::Get()->Unregister(this, &TEffectManager::OnUpdate);
	TEventRender2D::Get()->Unregister(this, &TEffectManager::OnRender);

	Reset();
}

//-------------------------------------------------------------------------------
void TEffectManager::Reset()
{
	for (unsigned int i=0; i<m_effects.size(); ++i)
	{
		TSafeDelete(m_effects[i]);
	}

	m_effects.clear();
	m_effectPositions.clear();
}

//-------------------------------------------------------------------------------
void TEffectManager::OnUpdate(float elapsedTime)
{
	for (unsigned int i=0; i<m_effects.size(); ++i)
	{
		m_effects[i]->Update(elapsedTime);

		if (m_effects[i]->HasAnimationFinished())
		{
			TSafeDelete(m_effects[i]);
			m_effects.erase( m_effects.begin() + i );
			m_effectPositions.erase( m_effectPositions.begin() + i );
		}
	}
}

//-------------------------------------------------------------------------------
void TEffectManager::OnRender()
{
	for (unsigned int i=0; i<m_effects.size(); ++i)
	{
		m_effects[i]->Render(m_effectPositions[i]);
	}
}

//-------------------------------------------------------------------------------
bool TEffectManager::AddEffect(const TVector2& position, const char *filename)
{
	TAnimatedSprite* effect = TNew TAnimatedSprite();

	if (!effect->Load(filename))
	{
		TDebugPrint(("[TEffectManager::AddEffect] Failed to load from %s", filename));
		TSafeDelete(effect);
		return false;
	}
	else
	{
		m_effects.push_back(effect);
		m_effectPositions.push_back(position);
		return true;
	}
}

//-------------------------------------------------------------------------------
bool TEffectManager::PreloadFromFile(const char* filename)
{
	// open file
	FILE* f = NULL;
	fopen_s(&f, filename, "r");

	if (!f)
	{
		TDebugPrint(("[TEffectManager::PreloadEffects] Couldn't open file %s", filename));
		return false;
	}

	// read lines until EOF
	char str[1024];

	int line = 0;
	TVector2 v(0,0);

	while (fgets(str, 1024, f))
	{
		// fetch next line
		std::string string = str;		// convert to std::string (easier)
		string.erase( string.end()-1 );	// cut newline

		UINT temp = 0;

		if (!AddEffect(v, string.c_str()))
		{
			TDebugPrint(("[TEffectManager::PreloadEffects]: preload failed (%s)", string.c_str()));
		}

		++line;
	}

	fclose(f);
	f = NULL;

	Reset();
	return true;
}
