////////////////////////////////////////////////////////////////////////////////
// Ui360Controller.h

#pragma once
#include "windows.h"
#include <XInput.h>
#include "Ui360.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtVector3.h"

struct Ui360ControllerState
{
	XINPUT_STATE    m_state;
	XINPUT_STATE    m_lastState;
	BtBool          m_bConnected;
};

// Class definition
class Ui360WinGL : public Ui360
{
public:
	
	// Constructor
	Ui360WinGL();
  
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

	BtFloat					GetNormalise( BtS32 value );

	Ui360ControllerState	m_controllers[MaxControllers];
	BtBool					m_bDeadZone;
};
