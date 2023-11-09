#ifndef __IAUDIO_H__
#define __IAUDIO_H__

//+----------------------------------------
//| Base audio component.
//| Version 1.003
//+----------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef __TSOUNDHANDLEDEFINED__
#define __TSOUNDHANDLEDEFINED__
#define TSoundHandle unsigned int
#endif

class IAudio
{
public:
							IAudio() {}
	virtual					~IAudio () {}

	virtual bool			LoadSound(const char* filename, TSoundHandle* nID, bool bLoop=false) = 0;
	virtual void			PlaySound(TSoundHandle nID) = 0;
	virtual void			StopSound(TSoundHandle nID) = 0;
	virtual void			StopAllSounds() = 0;
	virtual unsigned int	GetNumSounds() = 0;
	virtual bool			IsPlaying(TSoundHandle nID) = 0;

	// volume between [0.0, 1.0]
	virtual void			SetVolume(float fVolume) = 0;

	virtual void			OnLostDevice() = 0;
	virtual void			Update() = 0;
};

#endif
