//
//  testcontrollers.cpp
//  PSMoveOSX
//
//  Created by localadmin on 07/06/2018.
//

#include "BtBase.h"
#include "testControllers.hpp"
#include "BaArchive.h"
#include "BtTime.h"
#include "SdSound.h"
#include "ShIMU.h"

void TestControllers::setup()
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

void TestControllers::reset()
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

void TestControllers::update()
{
     for( BtU32 i=0; i<numControllers; i++ )
     {
         PSMove *move = moveArr[i];
         
         int res = psmove_poll( move );
         if (res)
         {
             #ifdef MADGWICK
                 MtQuaternion quaternion;
                 BtFloat fax, fay, faz;
                 BtFloat fgx, fgy, fgz;
             
                 for( BtU32 j=0; j<2; j++ )
                 {
                     PSMove_Frame frame = (PSMove_Frame)j;
                     psmove_get_accelerometer_frame( move, frame, &fax, &fay, &faz );
                     psmove_get_gyroscope_frame( move, frame, &fgx, &fgy, &fgz );
                     
                     Madgwick &madge = madgwick[i];
                     madge.MadgwickAHRSupdateIMU( fgx, fgy, fgz, fax, fay, faz );
                 }
             
                 // Works vertically with x, z, y
                 Madgwick &madge = madgwick[i];
                 quaternion = MtQuaternion( -madge.q1, -madge.q3, -madge.q2, madge.q0 );
                 ShIMU::SetQuaternion( 0, quaternion );
             #endif
             
             // Respond to the trigger
             int trigger = psmove_get_trigger( move );
             
             if( trigger != lastTrigger[i] )//(128 + 64 + 32 + 16) )
             {
                 psmove_set_leds( move, trigger, trigger, trigger );
                 lastTrigger[i] = trigger;
             }
             
             //
             int mx, my, mz;
             psmove_get_magnetometer( move, &mx, &my, &mz );
             float mag = mx + my + mz;
             if( mag > 16.0f )
             {
                 magnet[i] = BtTrue;
                 psmove_set_leds( move, 255, 255, 255 );
             }
             else
             {
                 if( magnet[i] )
                     psmove_set_leds( move, 0, 0, 0 );
                 magnet[i] = BtFalse;
             }
             
             // Respond to the buttons
             unsigned int pressed, released;
             psmove_get_button_events( move, &pressed, &released);
             if( pressed == Btn_TRIANGLE )
             {
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
                 // Integrate the battery level and set the colour from RED to WHITE
                 PSMove_Battery_Level batt = psmove_get_battery( move );
                 
                 switch( batt )
                 {
                     case Batt_MIN :
                         psmove_set_leds( move, 255, 0, 0 );
                     break;
                     case Batt_20Percent:
                         psmove_set_leds( move, 128, 128, 0 );
                     break;
                     case Batt_40Percent:
                         psmove_set_leds( move, 128, 200, 0 );
                     break;
                     case Batt_60Percent:
                         psmove_set_leds( move, 0, 200, 0 );
                     break;
                     case Batt_80Percent :
                         psmove_set_leds( move, 0, 220, 0 );
                     break;
                     case Batt_MAX:
                         psmove_set_leds( move, 0, 255, 0 );
                     break;
                     case Batt_CHARGING:
                         psmove_set_leds( move, 0, 0, 255 );
                     break;
                     case Batt_CHARGING_DONE:
                         psmove_set_leds( move, 255, 255, 255 );
                     break;
                 }
             }
             if( released )
             {
                 psmove_set_leds( move, 0, 0, 0 );
             }
             
             // Update any changes to the lights
             psmove_update_leds( move );
         }
     }
 }

