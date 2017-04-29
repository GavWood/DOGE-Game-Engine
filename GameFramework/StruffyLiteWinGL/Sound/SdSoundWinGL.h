////////////////////////////////////////////////////////////////////////////////
// SdSoundWinGL.h

#pragma once

// Includes
#include "BtArray.h"
#include "BaResource.h"
#include "BaFileData.h"
#include "SdSound.h"

#include "al.h"
#include "alc.h"

class BaArchive;
class SdSample;
struct BaSound;

// Class declaration
class SdSoundWinGL : public SdSound
{
public:

	// Constructor
	SdSoundWinGL();

	virtual void							FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	static void 							CreateManager();
	static void 							DestroyManager();

	void 									CreateOnDevice();
	void 									RemoveFromDevice();

	BtS32									Play( BtFloat pitch, BtFloat volume = 1.0f, BtBool loop = BtFalse );

	void									SetListenerPosition( const MtVector3 &v3Position );
	void									SetOrientation( const MtMatrix3 &m3Rotation );
	void									SetPosition( const MtVector3 &v3Position );

	void									SetVolume( BtS32 channel, BtFloat volume );

    // Accessors
	static void								Stop( BtS32 channel );
	static BtBool							IsPlaying( BtS32 channel );
	static void								SetEnabled( BtBool isEnabled );
	static BtBool							GetEnabled();

private:
    friend class SdSound;

	BtS32									GetFreeChannel();

	BaSound*								m_pSound;
	BtU8*									m_pRawData;
	BtU32									m_bufferIndex;

	static ALCdevice*						m_pALDevice;
	static ALCcontext*						m_pALContext;
	static BtU32							m_currentSound;
	static ALuint							m_buffer[MaxSamplesBuffered];
	static ALuint							m_source[MaxSoundsPlaying];
	static BtU32							m_currentBuffer;
	static BtBool							m_isEnabled;
};
