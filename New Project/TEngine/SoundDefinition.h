#ifndef __SOUNDDEFINITION_H__
#define __SOUNDDEFINITION_H__

//+----------------------------------------
//| Stores a single sound.
//| Version 1.001
//+----------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __DSOUND_INCLUDED__
#include <dsound.h>
#endif

class SoundWaveFile;

class SoundDefinition
{
public:
	SoundDefinition(const char* fileName, bool loop);
	~SoundDefinition();

	bool			CreateSoundBuffer();
	void			Play(int volume);
	void			SetVolume(int volume);
	void			Stop();
	void			Update();
	bool			IsPlaying() const;

	const TString& 	GetFileName();
	bool			GetLooping();
	void*			GetSharedBuffer();

	void			FillSharedBuffer(SoundWaveFile& loader);
	void			OnLostDevice();

private:
	TString		m_fileName;
	bool		m_loop;
	int			m_bufferSize;
	void*		m_sharedBuffer;
	int			m_volume;
	bool		m_isPlaying;

	LPDIRECTSOUNDBUFFER		m_soundBuffer;
};

#endif
