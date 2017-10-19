////////////////////////////////////////////////////////////////////////////////
// SdSoundImpl.cpp

// Includes
#include "SdSoundImpl.h"
#include "SdSoundCaptureImpl.h"
#include "ErrorLog.h"
#include "ApConfig.h"

//////////////////////////////////////////////////////////////////////////
// Statics

ALCdevice*  SdSoundImpl::m_pALDevice;
ALCcontext* SdSoundImpl::m_pALContext;
BtU32 SdSoundImpl::m_currentSound = 0;
ALuint SdSoundImpl::m_source[MaxSoundsPlaying];
BtU32 SdSoundImpl::m_currentBuffer = 0;
ALuint SdSoundImpl::m_buffer[MaxSamplesBuffered];

ALfloat sourcePos[3] = { 0.0, 0.0, 0.0 };
ALfloat sourceVel[3] = { 0.0, 0.0, 0.0 };
ALfloat sourceOri[3] = { 0.0, 0.0, 0.0 }; 

ALfloat listenerPos[3] = { 0.0, 0.0, 0.0 };
ALfloat listenerVel[3] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[3] = { 0.0, 0.0, 0.0 };

//static
BtBool SdSoundImpl::m_isEnabled = BtTrue;

SdSoundCaptureAL g_captureThread;

////////////////////////////////////////////////////////////////////////////////
// checkError

BtBool checkError( const BtChar* msg )
{ 
	ALuint error = alGetError();
	(void)error;
	
	if( error != 0 )
	{
		switch( error )
		{
			case AL_NO_ERROR:
				BtPrint( "AL_NO_ERROR" );
				break;

			case AL_INVALID_NAME:
				BtPrint( "AL_INVALID_NAME");
				break;

			case AL_INVALID_ENUM:
				BtPrint("AL_INVALID_ENUM");
				break;

			case AL_INVALID_VALUE:
				BtPrint("AL_INVALID_VALUE");
				break;

			case AL_INVALID_OPERATION:
				BtPrint("AL_INVALID_OPERATION");
				break;

			case AL_OUT_OF_MEMORY:
				BtPrint("AL_OUT_OF_MEMORY");
				break;
		}
	}

	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// GetVolume

//static
BtFloat SdSound::GetVolume()
{
	return SdSoundCaptureAL::GetVolume();
}

////////////////////////////////////////////////////////////////////////////////
// GetPitch

BtFloat SdSound::GetPitch()
{
	return SdSoundCaptureAL::GetPitch();
}

////////////////////////////////////////////////////////////////////////////////
// GetFFT

BtFloat *SdSound::GetFFT( BtU32 &samples )
{
	return SdSoundCaptureAL::GetFFT( samples );
}

////////////////////////////////////////////////////////////////////////////////
// StartCapture

// static
void SdSound::StartCapture( BtBool isToFile )
{
    SdSoundCaptureAL::StartCapture(isToFile);
}

////////////////////////////////////////////////////////////////////////////////
// Update

// static
void SdSound::UpdateCapture()
{
    SdSoundCaptureAL::UpdateCapture();
}

////////////////////////////////////////////////////////////////////////////////
// StopCapture

// static
void SdSound::StopCapture()
{
    SdSoundCaptureAL::StopCapture();
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

SdSoundImpl::SdSoundImpl()
{
	m_currentSound = 0;
}

////////////////////////////////////////////////////////////////////////////////
// CreateManager

void SdSoundImpl::CreateManager()
{
    BtBool foundOpenAL = BtTrue;
    
    if( ApConfig::IsWin() )
    {
        if( alcIsExtensionPresent( BtNull, "ALC_ENUMERATE_ALL_EXT" ) == AL_TRUE )
        {
            //
            const ALCchar* pDefaultDevice = BtNull;

            // Get the pointer to the device lists
            pDefaultDevice = alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );

            if( pDefaultDevice == BtNull )
            {
                foundOpenAL = BtFalse;
            }
        }
        else
        {
            foundOpenAL = BtFalse;
        }
    }
    
    if( foundOpenAL )
    {
		// Initialise
		m_pALDevice = alcOpenDevice( BtNull );

		// Create the context
		m_pALContext = alcCreateContext( m_pALDevice, NULL );
		alcMakeContextCurrent( m_pALContext );

		// Set the listeners for the device
		alListenerfv( AL_POSITION,	  listenerPos ); 
		alListenerfv( AL_VELOCITY,	  listenerVel ); 
		alListenerfv( AL_ORIENTATION, listenerOri );

		// Create the source
		alGenSources( MaxSoundsPlaying, m_source );
		//checkError( "alGenSources");

		// Associate it with the buffer
		for( BtU32 i=0; i<MaxSoundsPlaying; i++ )
		{
			// Set the position of this source
			alSourcefv( m_source[i], AL_POSITION,  sourcePos ); 
			alSourcefv( m_source[i], AL_VELOCITY,  sourceVel ); 
			alSourcefv( m_source[i], AL_DIRECTION, sourceOri ); 
		}

		alGenBuffers( MaxSamplesBuffered, &m_buffer[0] );
		checkError( "alGenBuffers"	);

		m_currentBuffer = 0;
	}
	else
	{
		BtPrint( "Could not find OpenAL extension\r\n" );
	}
}

////////////////////////////////////////////////////////////////////////////////
// DestroyManager

void SdSoundImpl::DestroyManager()
{
	alDeleteSources( MaxSoundsPlaying, m_source );
	alDeleteBuffers( MaxSamplesBuffered, m_buffer );
	m_currentBuffer = 0;

	alcMakeContextCurrent( NULL );
	alcDestroyContext( m_pALContext );
	alcCloseDevice( m_pALDevice );
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

//virtual
void SdSoundImpl::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	m_pSound   = (BaSound*)pFileData;
	m_pRawData = (BtU8*)( m_pSound + 1 );
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void SdSoundImpl::CreateOnDevice()
{
	ALenum format;

	if( m_pSound->m_nBitsPerSample == 16 )
	{
		switch( m_pSound->m_nChannels )
		{
			case 1:
				format = AL_FORMAT_MONO16;
				break;
			case 2:
				format = AL_FORMAT_STEREO16;
				break;
		}
	}
	else
	{
		switch( m_pSound->m_nChannels )
		{
			case 1:
				format = AL_FORMAT_MONO8;
				break;
			case 2:
				format = AL_FORMAT_STEREO8;
				break;
		}
	}

	ALsizei    size    = m_pSound->m_nSize;
	ALsizei    freq    = m_pSound->m_nSamplesPerSec;
	ALvoid*    data    = (ALvoid*)m_pRawData;

	m_bufferIndex = m_currentBuffer;

	++m_currentBuffer;

	// Create a buffer
	alBufferData( m_buffer[m_bufferIndex], format, data, size, freq );
	checkError( "alBufferData");
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void SdSoundImpl::RemoveFromDevice()
{
}

////////////////////////////////////////////////////////////////////////////////
// IsPlaying

BtBool SdSoundImpl::IsPlaying( BtS32 channel )
{
	ALint state;

	ALuint source = m_source[channel];

	alGetSourcei( source, AL_SOURCE_STATE, &state );

	switch( state )
	{
	case AL_PLAYING:

		return BtTrue;
		break;
	}
	
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// SetVolume

void SdSoundImpl::SetVolume( BtS32 channel, BtFloat volume )
{
	ALuint source = m_source[channel];

	alSourcef( source, AL_GAIN, volume );
}

////////////////////////////////////////////////////////////////////////////////
// GetFreeChannel

BtS32 SdSoundImpl::GetFreeChannel()
{
	ALint state;

	for( BtU32 i=0; i<MaxSoundsPlaying; i++ )
	{
		ALuint source = m_source[i];

		alGetSourcei( source, AL_SOURCE_STATE, &state );

		switch( state )
		{
			case AL_INITIAL:
			case AL_STOPPED:

				return i;
				break;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Stop

void SdSoundImpl::Stop( BtS32 channel )
{
	ALuint source = m_source[channel];

	alSourceStop( source );
}

////////////////////////////////////////////////////////////////////////////////
// Play

BtS32 SdSoundImpl::Play( BtFloat pitch, BtFloat volume, BtBool loop )
{
	BtS32 channel = GetFreeChannel();

	if( channel != -1 )
	{
		ALuint source = m_source[channel];

		if( loop == BtTrue )
		{
			alSourcei( source, AL_LOOPING, AL_TRUE );
		}
		else
		{
			alSourcei( source, AL_LOOPING, AL_FALSE );
		}

		alSourcef( source, AL_GAIN, volume );
		alSourcei( source, AL_BUFFER, m_buffer[m_bufferIndex] );
		checkError( "alSourcei");

		alSourcef( source, AL_PITCH, pitch );
		alSourceRewind( source );
		alSourcePlay( source );
		checkError( "Source play" );
	}
	return channel;
}

////////////////////////////////////////////////////////////////////////////////
// SetOrientation

void SdSoundImpl::SetOrientation( const MtMatrix3 &m3Rotation )
{

}

////////////////////////////////////////////////////////////////////////////////
// SetListenerPosition

void SdSoundImpl::SetListenerPosition( const MtVector3& v3Position )
{
}

////////////////////////////////////////////////////////////////////////////////
// SetPosition

void SdSoundImpl::SetPosition( const MtVector3& v3Position )
{
}

////////////////////////////////////////////////////////////////////////////////
// SetEnabled

void SdSound::SetEnabled( BtBool isEnabled )
{
	SdSoundImpl::m_isEnabled = isEnabled;

	if( isEnabled == BtFalse )
	{	
		for( BtU32 i=0; i<MaxSoundsPlaying; i++ )
		{
			if( SdSoundImpl::IsPlaying( i ) )
			{
				SdSoundImpl::Stop( i );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Stop

void SdSound::Stop( BtS32 channel )
{
	SdSoundImpl::Stop( channel );
}

////////////////////////////////////////////////////////////////////////////////
// IsPlaying

BtBool SdSound::IsPlaying( BtS32 channel )
{
	return SdSoundImpl::IsPlaying( channel );
}

////////////////////////////////////////////////////////////////////////////////
// Stop

BtBool SdSound::GetEnabled()
{
	return SdSoundImpl::m_isEnabled;
}
