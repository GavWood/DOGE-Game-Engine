////////////////////////////////////////////////////////////////////////////////
// Ui360WinGL.cpp

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


Ui360ControllerState	m_controllers[MaxControllers];
BtBool m_bDeadZone = BtTrue;
const BtFloat f360DeadZone = 0.24f * BtFloat( 0x7fff );

////////////////////////////////////////////////////////////////////////////////
// Update

void Ui360::Update()
{
	DWORD dwResult;    
	for (DWORD i=0; i< MaxControllers; i++ )
	{
		m_controllers[i].m_lastState = m_controllers[i].m_state;

		// Cache the input state
		XINPUT_STATE& state = m_controllers[i].m_state;

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState( i, &state );

		if( dwResult == ERROR_SUCCESS )
		{
			// Flag as connected
			m_controllers[i].m_bConnected = BtTrue;

			// Make a dead zone
			if( m_bDeadZone )
			{
				// Zero value if thumb sticks are within the dead zone 
				if( ( state.Gamepad.sThumbLX < f360DeadZone && state.Gamepad.sThumbLX > -f360DeadZone ) && 
					( state.Gamepad.sThumbLY < f360DeadZone && state.Gamepad.sThumbLY > -f360DeadZone ) ) 
				{   
					state.Gamepad.sThumbLX = 0;
					state.Gamepad.sThumbLY = 0;
				}

				if( ( state.Gamepad.sThumbRX < f360DeadZone && state.Gamepad.sThumbRX > -f360DeadZone ) && 
					( state.Gamepad.sThumbRY < f360DeadZone && state.Gamepad.sThumbRY > -f360DeadZone ) ) 
				{
					state.Gamepad.sThumbRX = 0;
					state.Gamepad.sThumbRY = 0;
				}
			}
		}
		else
		{
			m_controllers[i].m_bConnected = BtFalse;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNormalise

BtFloat GetNormalise( BtS32 value )
{
	BtFloat fValue = (BtFloat) value;

	fValue = fValue / 32768.0f;

	return fValue;
}

////////////////////////////////////////////////////////////////////////////////
// LeftThumb

MtVector2 Ui360::LeftThumb( BtU32 iIndex )
{
	MtVector2 v3Left = MtVector2( 0, 0 );

	if( m_controllers[iIndex].m_bConnected == BtTrue )
	{
		// Cache the input state
		XINPUT_STATE& state = m_controllers[iIndex].m_state;

		v3Left = MtVector2( GetNormalise( state.Gamepad.sThumbLX ), GetNormalise( state.Gamepad.sThumbLY ) );
	}

	return v3Left;
}

////////////////////////////////////////////////////////////////////////////////
// RightThumb

MtVector2 Ui360::RightThumb( BtU32 iIndex )
{
	MtVector2 v3Right = MtVector2( 0, 0 );

	if( m_controllers[iIndex].m_bConnected == BtTrue )
	{
		// Cache the input state
		XINPUT_STATE& state = m_controllers[iIndex].m_state;

		v3Right = MtVector2( GetNormalise( state.Gamepad.sThumbRX ), GetNormalise( state.Gamepad.sThumbRY ) );
	}

	return v3Right;
}

////////////////////////////////////////////////////////////////////////////////
// Connected

BtBool Ui360::IsConnected( BtU32 iIndex )
{
	return m_controllers[iIndex].m_bConnected;
}

////////////////////////////////////////////////////////////////////////////////
// LeftTrigger

BtFloat Ui360::LeftTrigger( BtU32 iIndex )
{
	BtFloat fValue = (BtFloat) m_controllers[iIndex].m_state.Gamepad.bLeftTrigger;
	fValue = fValue / 255.0f;

	return fValue;
}

////////////////////////////////////////////////////////////////////////////////
// RightTrigger

BtFloat Ui360::RightTrigger( BtU32 iIndex )
{
	BtFloat fValue = (BtFloat) m_controllers[iIndex].m_state.Gamepad.bRightTrigger;
	fValue = fValue / 255.0f;

	return fValue;
}

////////////////////////////////////////////////////////////////////////////////
// Buttons

BtU32 Ui360::Buttons( BtU32 iIndex )
{
	return m_controllers[iIndex].m_state.Gamepad.wButtons;
}

////////////////////////////////////////////////////////////////////////////////
// ButtonReleased

BtBool Ui360::ButtonReleased( BtU32 iIndex, BtU32 mask )
{
	BtU32 flag1 = m_controllers[iIndex].m_state.Gamepad.wButtons & mask;
	BtU32 flag2 = m_controllers[iIndex].m_lastState.Gamepad.wButtons & mask;

	if( !flag1 && flag2 )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// ButtonPressed

BtBool Ui360::ButtonPressed( BtU32 iIndex, BtU32 mask )
{
	BtU32 flag1 = m_controllers[iIndex].m_state.Gamepad.wButtons & mask;
	BtU32 flag2 = m_controllers[iIndex].m_lastState.Gamepad.wButtons & mask;

	if( flag1 && !flag2 )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// ButtonHeld

BtBool Ui360::ButtonHeld( BtU32 iIndex, BtU32 mask )
{
	BtU32 flag1 = m_controllers[iIndex].m_state.Gamepad.wButtons & mask;

	if( flag1 )
	{
		return BtTrue;
	}
	return BtFalse;
}
