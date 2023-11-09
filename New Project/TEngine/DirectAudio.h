#ifndef __TAUDIO_H__
#define __TAUDIO_H__

//+-------------------------------------------------------
//| D3D Audio component
//| Used to store and play sounds using DirectSound.
//| Version 1.005
//+-------------------------------------------------------

#ifndef __IAUDIO_H__
#include "IAudio.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef __DSOUND_INCLUDED__
#include <dsound.h>
#endif

class SoundDefinition;

class DirectAudio : public IAudio
{
public:
							DirectAudio();
							~DirectAudio();

	static LPDIRECTSOUND &	GetSoundObject();

	bool					Init(HWND g_hWindow);

	bool					LoadSound(const char* filename, TSoundHandle* nID, bool bLoop=false);
	void					PlaySound(TSoundHandle nID);
	void					StopSound(TSoundHandle nID);
	void					SetVolume(float fVolume);
	void					StopAllSounds();
	unsigned int			GetNumSounds();
	bool					IsPlaying(TSoundHandle nID);

	void					Update();
	void					OnLostDevice();
private:
	void						Release();

	HWND						m_hWndMain;

	/* --- DirectSound --- */
	static LPDIRECTSOUND		m_directSound;
	LPDIRECTSOUNDBUFFER			m_primarySoundBuffer;
	void*						m_listener;

	/* --- Sound Objects --- */
	std::vector<SoundDefinition*> m_pSounds;

	float						m_fVolume;
	int							m_nDirectVolume;
}; // TAudio

#endif
