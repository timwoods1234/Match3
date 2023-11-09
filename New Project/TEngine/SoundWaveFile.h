#ifndef __SOUNDWAVEFILE_H__
#define __SOUNDWAVEFILE_H__

//+----------------------------------------
//| Wrapper for wave data (.wav file).
//| Version 1.000
//+----------------------------------------

#ifndef _WINDOWS_
#include <windows.h>
#endif

class SoundWaveFile
{
public:
			SoundWaveFile(const char* fileName);
			~SoundWaveFile();
	bool	isVaild() const;
	bool	Read(int size,BYTE* data,UINT* sizeRead);
	void	Close();
	bool	Reset();
	void*	GetWaveFormat();
	void*	GetWaveRiffChunk();
	void*	GetWaveInfo();
private:
	bool	m_valid;
	void*	m_waveFormat;
	void*	m_mmIOHandle;
	int		m_ckIn[5];
	int		m_ckInRiff[5];
};

#endif