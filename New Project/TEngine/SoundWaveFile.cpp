#include "SoundWaveFile.h"
#include "TError.h"

//-------------------------------------------------------------------------------
bool ReadMMIO( HMMIO hmmioIn, MMCKINFO* pckInRIFF, WAVEFORMATEX** ppwfxInfo )
{
	MMCKINFO        ckIn;           // chunk info. for general use.
	PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.       

	*ppwfxInfo = NULL;

	if( ( 0 != mmioDescend( hmmioIn, pckInRIFF, NULL, 0 ) ) )
		return false;

	if( (pckInRIFF->ckid != FOURCC_RIFF) ||
		(pckInRIFF->fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
		return false;

	// Search the input file for for the 'fmt ' chunk.
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( 0 != mmioDescend(hmmioIn, &ckIn, pckInRIFF, MMIO_FINDCHUNK) )
		return false;

	// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
	// if there are extra parameters at the end, we'll ignore them
	if( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
		return false;

	// Read the 'fmt ' chunk into <pcmWaveFormat>.
	if( mmioRead( hmmioIn, (HPSTR) &pcmWaveFormat, 
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat) )
		return false;

	// Allocate the waveformatex, but if its not pcm format, read the next
	// word, and thats how many extra bytes to allocate.
	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		if( NULL == ( *ppwfxInfo = new WAVEFORMATEX ) )
			return false;

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy( *ppwfxInfo, &pcmWaveFormat, sizeof(pcmWaveFormat) );
		(*ppwfxInfo)->cbSize = 0;
	}
	else
	{
		// Read in length of extra bytes.
		WORD cbExtraBytes = 0L;
		if( mmioRead( hmmioIn, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD) )
			return false;

		*ppwfxInfo = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];
		if( NULL == *ppwfxInfo )
			return false;

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy( *ppwfxInfo, &pcmWaveFormat, sizeof(pcmWaveFormat) );
		(*ppwfxInfo)->cbSize = cbExtraBytes;

		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		if( mmioRead( hmmioIn, (CHAR*)(((BYTE*)&((*ppwfxInfo)->cbSize))+sizeof(WORD)),
			cbExtraBytes ) != cbExtraBytes )
		{
			delete( *ppwfxInfo);
			*ppwfxInfo = NULL;
			return false;
		}
	}

	// Ascend the input file out of the 'fmt ' chunk.
	if( 0 != mmioAscend( hmmioIn, &ckIn, 0 ) )
	{
		delete( *ppwfxInfo);
		*ppwfxInfo = NULL;
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
bool WaveOpenFile( CHAR* strFileName, HMMIO* phmmioIn, WAVEFORMATEX** ppwfxInfo, MMCKINFO* pckInRIFF)
{
	bool hr;
	HMMIO   hmmioIn = NULL;

	if( NULL == ( hmmioIn = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF|MMIO_READ ) ) )
	{
		return false;
	}

	if( FAILED( hr = ReadMMIO( hmmioIn, pckInRIFF, ppwfxInfo ) ) )
	{
		mmioClose( hmmioIn, 0 );
		return hr;
	}

	*phmmioIn = hmmioIn;

	return true;
}

//-------------------------------------------------------------------------------
bool WaveStartDataRead( HMMIO* phmmioIn, MMCKINFO* pckIn, MMCKINFO* pckInRIFF )
{
	// Seek to the data
	if( -1 == mmioSeek( *phmmioIn, pckInRIFF->dwDataOffset + sizeof(FOURCC),
		SEEK_SET ) )
	{
		return false;
	}

	// Search the input file for for the 'data' chunk.
	pckIn->ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( 0 != mmioDescend( *phmmioIn, pckIn, pckInRIFF, MMIO_FINDCHUNK ) )
	{
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
bool WaveReadFile( HMMIO hmmioIn, UINT cbRead, BYTE* pbDest,MMCKINFO* pckIn, UINT* cbActualRead )
{
	MMIOINFO mmioinfoIn;         // current status of <hmmioIn>

	*cbActualRead = 0;

	if( 0 != mmioGetInfo( hmmioIn, &mmioinfoIn, 0 ) )
		return false;

	UINT cbDataIn = cbRead;
	if( cbDataIn > pckIn->cksize ) 
		cbDataIn = pckIn->cksize;       

	pckIn->cksize -= cbDataIn;

	for( DWORD cT = 0; cT < cbDataIn; cT++ )
	{
		// Copy the bytes from the io to the buffer.
		if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
		{
			if( 0 != mmioAdvance( hmmioIn, &mmioinfoIn, MMIO_READ ) )
				return false;

			if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
				return false;
		}

		// Actual copy.
		*((BYTE*)pbDest+cT) = *((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

	if( 0 != mmioSetInfo( hmmioIn, &mmioinfoIn, 0 ) )
		return false;

	*cbActualRead = cbDataIn;
	return true;
}

//-------------------------------------------------------------------------------
SoundWaveFile::SoundWaveFile(const char* filename)
{
	m_valid = true;	

	if( FAILED( WaveOpenFile( (char*)filename, (HMMIO*)&m_mmIOHandle, (WAVEFORMATEX**)&m_waveFormat, (MMCKINFO*)&m_ckInRiff ) ) )
	{
		TDebugPrint(("[SoundWaveFile] Failed to open %s", filename));
		m_valid = false;	
	}

	if( m_valid)
	{
		Reset();
	}
}

//-------------------------------------------------------------------------------
SoundWaveFile::~SoundWaveFile()
{
	Close();
	delete(m_waveFormat);

}

//-------------------------------------------------------------------------------
bool SoundWaveFile::isVaild() const
{
	return m_valid;
}

//-------------------------------------------------------------------------------
bool SoundWaveFile::Reset()
{
	return !FAILED(WaveStartDataRead( (HMMIO*)&m_mmIOHandle, (MMCKINFO*)&m_ckIn, (MMCKINFO*)&m_ckInRiff ));
}

//-------------------------------------------------------------------------------
bool SoundWaveFile::Read( int size,BYTE* data,UINT* sizeRead)
{
	return !FAILED(WaveReadFile( (HMMIO)m_mmIOHandle, size, data, (MMCKINFO*)&m_ckIn, sizeRead ));
}

//-------------------------------------------------------------------------------
void SoundWaveFile::Close()
{
	mmioClose( (HMMIO)&m_mmIOHandle, 0 );
}

//-------------------------------------------------------------------------------
void* SoundWaveFile::GetWaveFormat()
{
	return m_waveFormat;
}

//-------------------------------------------------------------------------------
void* SoundWaveFile::GetWaveRiffChunk()
{
	return m_ckInRiff;
}

//-------------------------------------------------------------------------------
void* SoundWaveFile::GetWaveInfo()
{
	return m_ckIn;
}
