////////////////////////////////////////////////////////////////////////////////
// ExStrings

// Includes
#include "BtTypes.h"
#include "BtMemory.h"
#include "BaArchive.h"
#include "ExSound.h"
#include "BaArchive.h"
#include "SndIff.h"
#include "PaRendering.h"

#ifdef WIN32
#include "direct.h"
// For comparison
WAVEFORMATEX wavFormatEX;
BtU32 pcmFormat = WAVE_FORMAT_PCM;
#endif

typedef struct {
    BtU16  wFormatTag;
    BtU16  nChannels;
    BtU32  nSamplesPerSec;
    BtU32  nAvgBytesPerSec;
    BtU16  nBlockAlign;
    BtU16  wBitsPerSample;
    BtU16  cbSize;
} MYWAVEFORMATEX;

const BtU32 MYWAVE_FORMAT_PCM = 1;

////////////////////////////////////////////////////////////////////////////////
// Export

void ExSound::LoadSampleFile()
{
	BtChar szFilename[256];

	// Set the filename to the original sample
	sprintf( szFilename, "%s", GetFilename() );

	// Open the archive list
	FILE *f = fopen( szFilename, "rb" );

	// Don't load a dictionary if one is not present
	if( f == NULL )
	{
		return;
	}

	// Seek the end of the file
	fseek( f, 0, SEEK_END );

	// Get the position
	m_nFileSize = ftell( f );

	// Seek the start of the file
	fseek( f, 0, SEEK_SET );

	// Allocate the memory for the dictionary file
	m_pRawFileData = BtMemory::Allocate( m_nFileSize );

	// Read the file into the memory
	fread( m_pRawFileData, 1, m_nFileSize, f );

	// Close the archive list
	fclose( f );
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExSound::Export()
{
	PaMemoryResource resource(PMR_Write);

	SndIff iff;

	// Load the sample file into memory
	LoadSampleFile();

	// Read the sample file
	iff.Read( m_pRawFileData, m_nFileSize );

	// Calculate the resource size
	BaSound sound;

	// Set the format of the wave file
	BtMemory::Set( &sound, 0, sizeof(MYWAVEFORMATEX) );
    sound.m_wFormatTag      = MYWAVE_FORMAT_PCM;
	sound.m_nChannels       = iff.GetWavFormat().m_channels;
	sound.m_nSamplesPerSec  = iff.GetWavFormat().m_sampleRate;
	sound.m_nBitsPerSample  = iff.GetWavFormat().m_bitsPerSample;
	sound.m_nBlockAlign     = iff.GetWavFormat().m_blockAlign;
	sound.m_nAvgBytesPerSec = iff.GetWavFormat().m_averageBytesPerSec;
	sound.m_nSize			= iff.GetDataSize();

	resource << sound;
	resource << PaMemoryItem( iff.GetRawData(), iff.GetDataSize() );

	AddResource( resource, GetFilename(), GetTitle(), BaRT_Sound );

	// Free the sample
	free( m_pRawFileData );
}
