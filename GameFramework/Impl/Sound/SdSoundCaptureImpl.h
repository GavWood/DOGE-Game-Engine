//
//  SdSoundCaptureAL.h
//  wildman
//
//  Created by Gavin Wood on 17/03/2014.
//  Copyright (c) 2014 Gavin Wood. All rights reserved.
//

#include "BtArray.h"

class SdSoundCaptureAL
{
public:

    // Main interface
    static void                     StartCapture( BtBool isToFile, BtBool isFFT = BtFalse );
    static void                     UpdateCapture();
    static void                     StopCapture();

    
    // Accessors
    static BtFloat                  GetVolume();
    
    static BtFloat				   *GetFFT( BtU32 &samples );
	static BtFloat					GetPitch();
   
private:
    
	static BtBool					m_isFFT;
	static BtFloat					m_pitch;
};
