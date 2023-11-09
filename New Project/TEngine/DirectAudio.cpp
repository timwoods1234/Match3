#include "DirectAudio.h"
#include "SafeRelease.h"
#include "SoundDefinition.h"
#include "TMemory.h"
#include "TGeneralLib.h"

//-------------------------------------------------------------------------------
LPDIRECTSOUND DirectAudio::m_directSound;
static DS3DLISTENER gs_listenerParams;
#define pListener static_cast<LPDIRECTSOUND3DLISTENER>(m_listener)

//-------------------------------------------------------------------------------
DirectAudio::DirectAudio()
{
	m_directSound			= NULL;
	m_primarySoundBuffer	= NULL;

	m_fVolume				= 1.f;
	m_nDirectVolume			= 0;
}

//-------------------------------------------------------------------------------
DirectAudio::~DirectAudio()
{
	Release();
}

//-------------------------------------------------------------------------------
LPDIRECTSOUND& DirectAudio::GetSoundObject()
{
	return m_directSound;
}

//-------------------------------------------------------------------------------
bool DirectAudio::Init(HWND g_hWindow)
{
	// create device
	if (FAILED(DirectSoundCreate(NULL,&m_directSound,NULL)))
	{
		return false;
	}

	//set sound to DSSCL_PRIORITY
	if (FAILED( m_directSound->SetCooperativeLevel(g_hWindow, DSSCL_PRIORITY)))
	{
		return false;
	}

	DSBUFFERDESC dsbdesc;
	// create and alocate memory to the primary buffer
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));

	dsbdesc.dwSize  = sizeof( DSBUFFERDESC );
	dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

	// create buffer
	if (FAILED(m_directSound->CreateSoundBuffer(&dsbdesc,&m_primarySoundBuffer,NULL)))
	{
		return false;
	}

	if (FAILED(m_primarySoundBuffer->QueryInterface(IID_IDirectSound3DListener,&m_listener)))
	{
		return false;
	}

	// get listener parameters
	gs_listenerParams.dwSize = sizeof(DS3DLISTENER);
	pListener->GetAllParameters( &gs_listenerParams);

	// set sound quality
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx,sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = 22050;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if (FAILED(m_primarySoundBuffer->SetFormat(&wfx)))
	{
		return false;
	}

	return true;
} // Init

//-------------------------------------------------------------------------------
void DirectAudio::Release()
{
	// delete sounds
	for (unsigned int i=0; i<m_pSounds.size(); ++i)
	{
		m_pSounds[i]->Stop();
		TSafeDelete(m_pSounds[i]);
	}

	m_pSounds.clear();

	// delete DirectSound objects
	if (m_listener)
	{
		pListener->Release();
		m_listener = 0;
	}

	SafeRelease(m_primarySoundBuffer);
	SafeRelease(m_directSound);
} // Release

//-------------------------------------------------------------------------------
bool DirectAudio::LoadSound(const char* filename, TSoundHandle* nID, bool bLoop)
{
	// already loaded this file?
	for (unsigned int i=0; i<m_pSounds.size(); ++i)
	{
		if (m_pSounds[i]->GetFileName() == filename)
		{
			*nID = i;
			return true;
		}
	}

	SoundDefinition *sd = TNew SoundDefinition(filename, bLoop);

	(*nID) = m_pSounds.size();
	m_pSounds.push_back(sd);

	return true;
} // LoadSound

//-------------------------------------------------------------------------------
void DirectAudio::SetVolume(float fVolume)
{
	m_fVolume		= fVolume;

	m_nDirectVolume = (fVolume == 0.f)
					  ? DSBVOLUME_MIN
					  : DSBVOLUME_MAX + (int)( (DSBVOLUME_MIN * 0.5) * (1.f - fVolume));

	for (unsigned int i=0; i<m_pSounds.size(); ++i)
	{
		m_pSounds[i]->SetVolume(m_nDirectVolume);
	}
} // SetVolume

//-------------------------------------------------------------------------------
void DirectAudio::PlaySound(TSoundHandle nID)
{
	if (nID >= m_pSounds.size())
	{
		OutputDebugString("[DirectAudio::PlaySound] Requested sound out of bounds\n");
		return;
	}

	if (m_pSounds[nID]->IsPlaying())
	{
		m_pSounds[nID]->Stop();
	}

	m_pSounds[nID]->Play(m_nDirectVolume);

} // PlaySound

//-------------------------------------------------------------------------------
void DirectAudio::StopSound(TSoundHandle nID)
{
	if (nID >= m_pSounds.size())
	{
		OutputDebugString("[DirectAudio::StopSound] Requested sound out of bounds\n");
		return;
	}

	m_pSounds[nID]->Stop();
} // StopSound

//-------------------------------------------------------------------------------
void DirectAudio::Update()
{
	for (unsigned int i=0; i<m_pSounds.size(); ++i)
	{
		m_pSounds[i]->Update();
	}
}

//-------------------------------------------------------------------------------
void DirectAudio::OnLostDevice()
{
	for (unsigned int i=0; i<m_pSounds.size(); ++i)
	{
		m_pSounds[i]->OnLostDevice();
	}
}

//-------------------------------------------------------------------------------
void DirectAudio::StopAllSounds()
{
	for (unsigned int i=0; i<m_pSounds.size(); ++i)
	{
		m_pSounds[i]->Stop();
	}
}

//-------------------------------------------------------------------------------
unsigned int DirectAudio::GetNumSounds()
{
	return m_pSounds.size();
}

//-------------------------------------------------------------------------------
bool DirectAudio::IsPlaying(TSoundHandle nID)
{
	if (nID >= m_pSounds.size())
	{
		OutputDebugString("[DirectAudio::IsPlaying] Requested sound out of bounds\n");
		return false;
	}

	return m_pSounds[nID]->IsPlaying();
}
