//
//  SdSoundCaptureAL.cpp
//
//  Created by Gavin Wood on 17/03/2014.
//  Copyright (c) 2014 Gavin Wood. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "SdSoundCaptureImpl.h"
#include "BtTypes.h"
#include "al.h"
#include "alc.h"
#include "ErrorLog.h"
#include "MtMath.h"
#include "SdSound.h"

//#include "kiss_fft.h"
#include "fft.h"

#define FFTBufferSize 1024
// Wikipedia: In terms of frequency, human voices are roughly in the range of 
// 80 Hz to 1100 Hz (that is, E2 to C6).
const float MIN_FREQ = 50.0f;
//const float MAX_FREQ = 1500.0f;
const float MAX_FREQ = 1800.0f;
BtFloat SdSoundCaptureAL::m_pitch = 0;

#pragma pack (1)

typedef int32_t DWORD;
typedef int16_t WORD;

typedef struct tWAVEFORMATEX {
    WORD  wFormatTag;
    WORD  nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
    WORD  cbSize;
} WAVEFORMATEX;

typedef struct
{
	char			szRIFF[4];
	long			lRIFFSize;
	char			szWave[4];
	char			szFmt[4];
	long			lFmtSize;
	WAVEFORMATEX	wfex;
	char			szData[4];
	long			lDataSize;
} WAVEHEADER;
#pragma pack (8)

WAVEHEADER sWaveHeader;
ALCdevice* pCaptureDevice = NULL;
ALchar inWAV[FFTBufferSize];
FILE *audioFile;
BtU32 iDataSize;
BtFloat volume;

BtBool SdSoundCaptureAL::m_isFFT = BtFalse;
BtU32 SamplesRead = 0;

// alcMakeContextCurrent(NULL) before alcDestroyContext(outputContext).

///////////////////////////////////////////////////////////////////////////////
// StartCapture

// static
void SdSoundCaptureAL::StartCapture( BtBool isToFile, BtBool isFFT )
{
	// http://stackoverflow.com/questions/3056113/recording-audio-with-openal
	// http://stackoverflow.com/questions/9785447/simultaneous-record-playing-with-openal-using-threads

	pCaptureDevice = alcCaptureOpenDevice( NULL, 22050, AL_FORMAT_MONO16, FFTBufferSize);
    
	if( !pCaptureDevice  )
	{
		ErrorLog::Fatal_Printf( "Could not find EXT capture\r\n" );
		return;
	}
    
	ALCdevice		*pDevice;
	ALCcontext		*pContext;
	ALint			iDataSize = 0;
    
	// Check for Capture Extension support
	pContext = alcGetCurrentContext();
	pDevice = alcGetContextsDevice(pContext);
	if (alcIsExtensionPresent(pDevice, "ALC_EXT_CAPTURE") == AL_FALSE)
	{
		ErrorLog::Fatal_Printf( "Could not find EXT capture\r\n" );
		return;
	}
    
	// Create / open a file for the captured data
	audioFile = fopen( "C:\\temp\\sample.wav", "wb");
    
	if ( audioFile == NULL )
	{
		//ErrorLog::Fatal_Printf( "Could not find EXT capture\r\n" );
		//return;
	}
    
	audioFile = NULL;
    
	iDataSize = 0;
    
	// Prepare a WAVE file header for the captured data
	sprintf(sWaveHeader.szRIFF, "RIFF");
	sWaveHeader.lRIFFSize = 0;
	sprintf(sWaveHeader.szWave, "WAVE");
	sprintf(sWaveHeader.szFmt, "fmt ");
	sWaveHeader.lFmtSize = sizeof(WAVEFORMATEX);
	sWaveHeader.wfex.nChannels = 1;
	sWaveHeader.wfex.wBitsPerSample = 16;
	sWaveHeader.wfex.wFormatTag = 1; //WAVE_FORMAT_PCM;
	sWaveHeader.wfex.nSamplesPerSec = 22050;
	sWaveHeader.wfex.nBlockAlign = sWaveHeader.wfex.nChannels * sWaveHeader.wfex.wBitsPerSample / 8;
	sWaveHeader.wfex.nAvgBytesPerSec = sWaveHeader.wfex.nSamplesPerSec * sWaveHeader.wfex.nBlockAlign;
	sWaveHeader.wfex.cbSize = 0;
	sprintf(sWaveHeader.szData, "data");
	sWaveHeader.lDataSize = 0;
    
	if( audioFile )
	{
		fwrite(&sWaveHeader, sizeof(WAVEHEADER), 1, audioFile);
	}
    
	// Start audio capture
	alcCaptureStart(pCaptureDevice);

	m_isFFT= isFFT;

	init_fft( FFTBufferSize, 22050);
}

////////////////////////////////////////////////////////////////////////////////
// UpdateCapture

// static
void SdSoundCaptureAL::UpdateCapture()
{
	ALint iSamplesAvailable;
    
	// Find out how many samples have been captured
	alcGetIntegerv(pCaptureDevice, ALC_CAPTURE_SAMPLES, 1, &iSamplesAvailable);
    
	// When we have enough data to fill our BUFFERSIZE byte buffer, grab the samples
	if (iSamplesAvailable > (FFTBufferSize / sWaveHeader.wfex.nBlockAlign))
	{
		BtU32 numSamples = FFTBufferSize / sWaveHeader.wfex.nBlockAlign;

		// Consume Samples
		alcCaptureSamples(pCaptureDevice, inWAV, numSamples );
        
		ALCshort *buffer = (ALCshort*)inWAV;

		// Get the average volume
		BtU32 averageVolume = 0;
		for( BtU32 i=0; i<numSamples; i++ )
		{
			BtS32 val = buffer[i];
			averageVolume += MtAbs( val );
		}
		volume = ((BtFloat)averageVolume) / numSamples;
        
		SamplesRead = numSamples;

		// Calculate the pitch
		m_pitch = find_pitch( (BtU8*)buffer, MIN_FREQ, MAX_FREQ );

		// Write this to a file
		if( audioFile )
		{
			// Write the audio data to a file
			fwrite(inWAV, FFTBufferSize, 1, audioFile );
		}
        
		// Record total amount of data recorded
		iDataSize += FFTBufferSize;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetFFT

BtFloat	*SdSoundCaptureAL::GetFFT( BtU32 &samples )
{
	samples = SamplesRead;
	return (BtFloat*)BtNull;//simpleOutFFT;
}

////////////////////////////////////////////////////////////////////////////////
// GetPitch

BtFloat SdSoundCaptureAL::GetPitch()
{
	return m_pitch;
}

////////////////////////////////////////////////////////////////////////////////
// GetVolume

//static
BtFloat SdSoundCaptureAL::GetVolume()
{
	return volume;
}

////////////////////////////////////////////////////////////////////////////////
// StopCapture

void SdSoundCaptureAL::StopCapture()
{
	// Stop the FFT
	done_fft();

	// Stop capture
	alcCaptureStop(pCaptureDevice);
    
	// Fill in Size information in Wave Header
	if( audioFile )
	{
		fseek( audioFile, 4, SEEK_SET);
		BtU32 iSize = iDataSize + sizeof(WAVEHEADER) - 8;
		fwrite( &iSize, 4, 1, audioFile );
		fseek( audioFile, 42, SEEK_SET );
		fwrite( &iDataSize, 4, 1, audioFile );
        
		// Close the audio file
		fclose( audioFile );
	}
}
