//
//  SdSoundCaptureAL.h
//
//  Created by Gavin Wood on 17/03/2014.
//  Copyright (c) 2014 Gavin Wood. All rights reserved.
//

#include "BtArray.h"
#include "BtThread.h"

class SdSoundCaptureAL : public BtThread
{
public:
    
    static BtFloat                  GetVolume();
    
    static BtBool                   StartCapture( BtBool isToFile, BtBool isFFT = BtFalse );
	static void                     StopCapture();

    void							Create();
    void						    Destroy();
	void					        Update();
	
	static void                     Main();
	static BtFloat				   *GetFFT( BtU32 &samples );
	static BtFloat					GetPitch();
	static BtBool					IsRecordAvailable();

private:
    
    static BtBool                   m_isFFT;
	static BtFloat					m_pitch;
	static BtBool                   m_isRecordAvailable;
    static BtBool                   m_isRecord;
    static BtBool                   m_isUpdate;
};
