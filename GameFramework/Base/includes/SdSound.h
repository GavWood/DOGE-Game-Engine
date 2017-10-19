////////////////////////////////////////////////////////////////////////////////
// SdSound.h

#pragma once

// Includes
#include "BaResource.h"
#include "BaArchive.h"
#include "MtVector2.h"

const BtU32 MaxSoundsPlaying = 8;
const BtU32 MaxSamplesBuffered = 32;

// Class declaration
class SdSound : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Sound );

	// Capture and FFT
	static void				StartCapture( BtBool isToFile );
	static void				StopCapture();
    static void             UpdateCapture();
	static BtFloat			GetVolume();
	static BtFloat		   *GetFFT( BtU32 &samples );
	static BtFloat			GetPitch();

	// Audio playback
	virtual void			FixPointers( BtU8 *pFileData, BaArchive *pArchive ) = 0;

	virtual void 			CreateOnDevice() = 0;
	virtual void 			RemoveFromDevice() = 0;

	virtual BtS32			Play( BtFloat pitch = 1.0f, BtFloat volume = 1.0f, BtBool loop = BtFalse ) = 0;

	virtual void			SetVolume( BtS32 channel, BtFloat volume ) = 0;

	virtual void			SetOrientation( const MtMatrix3& m3Orientation ) = 0;
	virtual void			SetListenerPosition( const MtVector3& v3Position ) = 0;
	virtual void			SetPosition( const MtVector3& v3Position ) = 0;

	static void				Stop( BtS32 channel );
	static BtBool			IsPlaying( BtS32 channel );
	static BtBool			IsRecordAvailable();
	
	// Accessors
	static					void SetEnabled( BtBool isEnabled );
	static					BtBool GetEnabled();
};
