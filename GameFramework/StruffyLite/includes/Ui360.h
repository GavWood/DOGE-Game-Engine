////////////////////////////////////////////////////////////////////////////////
/// Ui360.h

#pragma once
#include "BtTypes.h"
#include "MtVector2.h"

const BtU32 MaxControllers = 4;
const BtU32 Ui360_Start = 16;
const BtU32 Ui360_Back = 32;
const BtU32 Ui360_LeftShoulder = 256;
const BtU32 Ui360_RightShoulder = 512;
const BtU32 Ui360_A = 0x1000;
const BtU32 Ui360_B = 0x2000;
const BtU32 Ui360_X = 0x4000;
const BtU32 Ui360_Y = 0x8000;
const BtU32 Ui360_Left  = 0x4;
const BtU32 Ui360_Right = 0x8;
const BtU32 Ui360_Down  = 0x2;
const BtU32 Ui360_Up    = 0x1;

// Class Declaration
class Ui360
{
public:

	// Instance
	static void					Update();

	// Accessors
	static MtVector2 			LeftThumb( BtU32 iIndex );
	static MtVector2 			RightThumb( BtU32 iIndex );
	static BtFloat				LeftTrigger( BtU32 iIndex );
	static BtFloat				RightTrigger( BtU32 iIndex );
	static BtU32				Buttons( BtU32 iIndex );
	static BtBool				ButtonPressed( BtU32 iIndex, BtU32 mask );
	static BtBool				ButtonReleased( BtU32 iIndex, BtU32 mask );
	static BtBool				ButtonHeld( BtU32 iIndex, BtU32 mask );
	static BtBool				IsConnected( BtU32 iIndex );
};
