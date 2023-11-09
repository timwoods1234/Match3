#include "SoundDefinition.h"
#include "DirectAudio.h"
#include "SoundWaveFile.h"
#include "TError.h"

//-------------------------------------------------------------------------------
SoundDefinition::SoundDefinition(const char* fileName, bool loop)
:	m_loop(loop),
	m_isPlaying(false),
	m_volume(100),
	m_sharedBuffer(0)
{
	m_soundBuffer = 0;
	m_fileName = fileName;

	SoundWaveFile soundWave(fileName);

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);

	dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;
	
	dsbd.dwBufferBytes = ((MMCKINFO*)soundWave.GetWaveRiffChunk())->cksize;
	dsbd.lpwfxFormat   = (WAVEFORMATEX*)soundWave.GetWaveFormat();

	if(FAILED(DirectAudio::GetSoundObject()->CreateSoundBuffer(&dsbd,(LPDIRECTSOUNDBUFFER*)&m_sharedBuffer,NULL)))
	{
		TDebugPrint(("[SoundDefinition::SoundDefinition] Failed to create sound buffer"));
		return;
	}

	m_bufferSize = dsbd.dwBufferBytes;
	FillSharedBuffer(soundWave);
}

//-------------------------------------------------------------------------------
SoundDefinition::~SoundDefinition()
{
	Stop();
}

//-------------------------------------------------------------------------------
bool SoundDefinition::CreateSoundBuffer()
{
	LPDIRECTSOUND dsound = DirectAudio::GetSoundObject();

	if(FAILED(dsound->DuplicateSoundBuffer((LPDIRECTSOUNDBUFFER)m_sharedBuffer,&m_soundBuffer)))
	{
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
void SoundDefinition::Play(int volume)
{
	if(!m_soundBuffer)
	{
		if(!CreateSoundBuffer())
		{
			return;
		}
	}

	SetVolume(volume);

	if (m_loop)
	{
		m_soundBuffer->Play(0,0,DSBPLAY_LOOPING);
	}
	else
	{
		m_soundBuffer->Play(0,0,NULL);
	}

	m_isPlaying = true;
}

//-------------------------------------------------------------------------------
void SoundDefinition::SetVolume(int volume)
{
	m_volume = volume;

	if(m_soundBuffer)
	{
		m_soundBuffer->SetVolume(m_volume);
		m_soundBuffer->SetPan(DSBPAN_CENTER);
	}		
}

//-------------------------------------------------------------------------------
void SoundDefinition::Stop()
{
	if(m_soundBuffer)
	{
		m_soundBuffer->Stop();
		m_soundBuffer->Release();
		m_soundBuffer = 0;
	}

	m_isPlaying = false;
}

//-------------------------------------------------------------------------------
void SoundDefinition::Update()
{
	if(m_soundBuffer)
	{
		DWORD status = 0;
		m_soundBuffer->GetStatus(&status);

		m_isPlaying =(( status & DSBSTATUS_PLAYING) != 0);
		if(!m_isPlaying)
		{
			Stop();
		}
	}
}

//-------------------------------------------------------------------------------
bool SoundDefinition::IsPlaying() const
{
	return m_isPlaying;
}

//-------------------------------------------------------------------------------
void SoundDefinition::FillSharedBuffer(SoundWaveFile& loader)
{
	BYTE*   pbWavData; 
	UINT    cbWavSize; 
	VOID*   pbData  = NULL;
	VOID*   pbData2 = NULL;
	DWORD   dwLength;
	DWORD   dwLength2;

	//creates the buffer the the right size.
	INT nWaveFileSize = ((MMCKINFO*)loader.GetWaveRiffChunk())->cksize;

	pbWavData = new BYTE[nWaveFileSize];

	if(FAILED(loader.Read(nWaveFileSize,pbWavData,&cbWavSize)))
	{
		return;
	}

	loader.Reset();

	//locks the buffer for writing
	if(FAILED(((LPDIRECTSOUNDBUFFER)m_sharedBuffer)->Lock( 0, m_bufferSize, &pbData, &dwLength, &pbData2, &dwLength2, 0L )))
	{
		return;
	}

	//Ensure that the end of the sound is not empty
	memcpy( pbData, pbWavData, m_bufferSize - 50 );

	((LPDIRECTSOUNDBUFFER)m_sharedBuffer)->Unlock( pbData, m_bufferSize, NULL, 0 );

	pbData = NULL;

	delete pbWavData;
}

//-------------------------------------------------------------------------------
void* SoundDefinition::GetSharedBuffer()
{
	return m_sharedBuffer;
}

//-------------------------------------------------------------------------------
const TString& SoundDefinition::GetFileName()
{
	return m_fileName;
}

//-------------------------------------------------------------------------------
void SoundDefinition::OnLostDevice()
{
	if (m_isPlaying == true)
	{
		bool loop = m_loop;
		Stop();
		m_loop = loop;
		Play(m_volume);
	}
}
