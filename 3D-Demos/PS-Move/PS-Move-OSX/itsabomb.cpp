//
//  itsabomb.cpp
//  PSMoveOSX
//
//  Created by localadmin on 07/06/2018.
//

#include "BtBase.h"
#include "itsabomb.hpp"
#include "BaArchive.h"
#include "BtTime.h"
#include "SdSound.h"
#include "ShIMU.h"

void ItsABombGame::setup()
{
    numControllers = psmove_count_connected();
    
    ShIMU::SetNumSensors( numControllers );
    
    printf("Connected PS Move controllers: %d\n", numControllers );
    
    // Connect all the controllers
    for( int i=0; i<numControllers; i++)
    {
        lastTrigger[i] = 0;
        
        moveArr[i] = psmove_connect_by_id(i);
        
        // Set the rumble to 0
        psmove_set_rumble( moveArr[i], 0 );
        
        // Set the lights to 0
        psmove_set_leds( moveArr[i], 0, 0, 0 );
        
        magnet[i] = BtFalse;
    }
    reset();
}

void ItsABombGame::reset()
{
#ifdef MADGWICK
    for( BtU32 i=0; i<numControllers; i++ )
    {
        Madgwick &madge = madgwick[i];
        madge.q0 = 1.0f; madge.q1 = madge.q2 = madge.q3 = 0;
        
        MtQuaternion quaternion = MtQuaternion( -madge.q1, -madge.q3, -madge.q2, madge.q0 );
        ShIMU::SetQuaternion( i, quaternion );
    }
#endif
}

void ItsABombGame::update()
{
    if( !BaArchive::GetArchives().pHead() )
    {
        return;
    }
    
    // Get the archive
    BaArchiveNode *archiveNode = BaArchive::GetArchives().pHead();
    BaArchive &archive = *archiveNode->m_pArchive;
    
    BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();
    
    if( m_reset )
    {
        startTime = BtTime::GetElapsedTimeInSeconds();
        currentTime = BtTime::GetElapsedTimeInSeconds();
        bang = BtFalse;
        m_reset = BtFalse;
        lastTime = 0;
        
        archive.GetSound("itsbomb")->Play();
    }
    
    BtFloat elapsedTime = (currentTime - startTime);
    BtU32 currentCount = (BtU32)(17.0f - elapsedTime);
    
    // Check for explosion
    
    //NSLog( @"ElapsedTime %.2f", elapsedTime);
    
    if( bang == BtFalse )
    {
        if( elapsedTime > 10 + 7)
        {
            archive.GetSound("bomb1")->Play();
            bang = BtTrue;
         }
        else if( currentCount != lastTime )
        {
            if( currentCount < 11 )
            {
                if( currentCount > 0 )
                {
                    BtChar text[32];
                    sprintf(text, "%d", currentCount );
                    archive.GetSound(text)->Play();
                }
                lastTime = currentCount;
            }
        }
    }
    
    for( BtU32 i=0; i<numControllers; i++ )
    {
        PSMove *move = moveArr[i];
        
        int res = psmove_poll( move );
        if (res)
        {
            // Respond to the buttons
            unsigned int pressed, released;
            psmove_get_button_events( move, &pressed, &released);
            if( pressed == Btn_TRIANGLE )
            {
                archive.GetSound("throw")->Play();
                psmove_set_leds( move, 0, 255, 0 );
            }
            if( pressed == Btn_CROSS )
            {
                psmove_set_leds( move, 0, 0, 255 );
            }
            if( pressed == Btn_CIRCLE )
            {
                psmove_set_leds( move, 255, 0, 0 );
            }
            if( pressed == Btn_TRIANGLE )
            {
                psmove_set_leds( move, 0, 255, 0 );
            }
            if( pressed == Btn_SQUARE )
            {
                psmove_set_leds( move, 0xFF, 0x69, 0xB4 );
            }
            if( pressed == Btn_MOVE )
            {
                m_reset = BtTrue;
            }
            
            // Update any changes to the lights
            psmove_update_leds( move );
        }
    }
}
