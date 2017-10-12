////////////////////////////////////////////////////////////////////////////////
// Ui360Impl.h

#pragma once

#include "Ui360.h"

// Class definition
class Ui360Impl : public Ui360
{
public:
	
	// Constructor
	Ui360Impl();
  
	void 					Update();

	MtVector2 				LeftThumb( BtU32 iIndex );
	MtVector2 				RightThumb( BtU32 iIndex );
	BtFloat					LeftTrigger( BtU32 iIndex );
	BtFloat					RightTrigger( BtU32 iIndex );
	BtU32					Buttons( BtU32 iIndex );
	BtBool					ButtonPressed( BtU32 iIndex, BtU32 mask );
	BtBool					ButtonReleased( BtU32 iIndex, BtU32 mask );
	BtBool					ButtonHeld( BtU32 iIndex, BtU32 mask );

	BtBool					IsConnected( BtU32 iIndex );

private:
};
