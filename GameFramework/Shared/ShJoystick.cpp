////////////////////////////////////////////////////////////////////////////////
// ShJoystick

// Includes
#include "ShJoystick.h"
#include "RsUtil.h"
#include "RsMaterial.h"
#include "UiKeyboard.h"
#include "BtString.h"
#include "ShTouch.h"

////////////////////////////////////////////////////////////////////////////////
// statics
ShJoystickInstance m_joystick[MaxJoysticks];
ShJoystick joysticks;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ShJoystick::ShJoystick()
{
	for (BtU32 controller = 0; controller < MaxJoysticks; controller++)
	{
		ShJoystickInstance &joystickInstance = m_joystick[controller];
	
		for (BtU32 index = 0; index < JoystickButton_MAX; index++)
		{
			joystickInstance.m_lastPressed[index] = BtFalse;
			joystickInstance.m_isHeld[index] = BtFalse;
			joystickInstance.m_isReleased[index] = BtFalse;
		}

		joystickInstance.m_m4Transform.SetIdentity();
		joystickInstance.m_m4Transform.SetTranslation( MtVector3( controller, 1.0f, 0 ) );
	}
}

//static
void ShJoystick::BeginTouch( BtU32 joystickIndex, ShJoystickButton button )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	joystickInstance.m_isHeld[button] = BtTrue;
}

//static
void ShJoystick::EndTouch( BtU32 joystickIndex, ShJoystickButton button )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	joystickInstance.m_isHeld[button] = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShJoystick::Update()
{
	// Work out whether buttons have been released
	for (BtU32 joystickIndex = 0; joystickIndex < MaxJoysticks; joystickIndex++)
	{
		// get the joystick instance
		ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];

		// Get the trigger for this joystick
		BtFloat trigger = GetTrigger(joystickIndex);
		if( trigger > 0.1f )
		{
			joystickInstance.m_isHeld[JoystickButton_Trigger] = BtTrue;
		}
		else
		{
			joystickInstance.m_isHeld[JoystickButton_Trigger] = BtFalse;
		}
	}

	// Work out whether buttons have been released (including the trigger)
	for( BtU32 joystickIndex = 0; joystickIndex < MaxJoysticks; joystickIndex++ )
	{
		ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];

		for (BtU32 i = 0; i < JoystickButton_MAX; i++)
		{
			joystickInstance.m_isPressed[i] = BtFalse;
			joystickInstance.m_isReleased[i] = BtFalse;

			if (joystickInstance.m_isHeld[i] == BtTrue)
			{
				if (joystickInstance.m_lastPressed[i] == BtFalse)
				{
					joystickInstance.m_isPressed[i] = BtTrue;
				}
				joystickInstance.m_lastPressed[i] = BtTrue;
			}
			else
			{
				if (joystickInstance.m_lastPressed[i] == BtTrue)
				{
					joystickInstance.m_isReleased[i] = BtTrue;
				}
				joystickInstance.m_lastPressed[i] = BtFalse;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsDirectionPressed

//static
BtBool ShJoystick::IsDirectionPressed(BtU32 joystickIndex )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];

    return joystickInstance.m_isPressed[JoystickButton_Up] ||
			joystickInstance.m_isPressed[JoystickButton_Down] ||
			joystickInstance.m_isPressed[JoystickButton_Left] ||
			joystickInstance.m_isPressed[JoystickButton_Right];
}

////////////////////////////////////////////////////////////////////////////////
// IsButtonPressed

//static
BtBool ShJoystick::IsButtonPressed( BtU32 joystickIndex )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];

    for( BtU32 i=0; i<JoystickButton_MAX; i++ )
    {
        if( joystickInstance.m_isPressed[i] )
        {
            return BtTrue;
        }
    }
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// SetConnected

//static
void ShJoystick::SetConnected( BtU32 joystickIndex, BtBool isConnected )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	joystickInstance.m_isUsed = BtTrue;
	joystickInstance.m_isConnected = isConnected;
}

////////////////////////////////////////////////////////////////////////////////
// SetUsed

//static
void ShJoystick::SetUsed( BtU32 joystickIndex, BtBool isUsed )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	joystickInstance.m_isUsed = isUsed;
}

////////////////////////////////////////////////////////////////////////////////
// IsUsed

//static
BtBool ShJoystick::IsUsed( BtU32 joystickIndex )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	return joystickInstance.m_isUsed;
}

////////////////////////////////////////////////////////////////////////////////
// IsConnected

//static
BtBool ShJoystick::IsConnected( BtU32 joystickIndex )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	return joystickInstance.m_isConnected;
}

////////////////////////////////////////////////////////////////////////////////
// IsReleased

//static
BtBool ShJoystick::IsReleased( BtU32 joystickIndex, ShJoystickButton button )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	return joystickInstance.m_isReleased[button];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

//static
BtBool ShJoystick::IsPressed( BtU32 joystickIndex, ShJoystickButton button )
{
	ShJoystickInstance &joystickInstance = m_joystick[joystickIndex];
	return joystickInstance.m_isPressed[button];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

//static
BtBool ShJoystick::IsPressed(BtU32 joystickIndex )
{
	for (BtU32 controller = 0; controller < MaxJoysticks; controller++)
	{
		ShJoystickInstance &joystickInstance = m_joystick[controller];

		for (BtU32 i = 0; i < JoystickButton_MAX; i++)
		{
			if (joystickInstance.m_isPressed[i] )
			{
				return BtTrue;
			}
		}
	}
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsHeld

//static
BtBool ShJoystick::IsHeld(BtU32 index, ShJoystickButton button )
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	return joystickInstance.m_isHeld[button];
}

////////////////////////////////////////////////////////////////////////////////
// SetLeftThumbStick

void ShJoystick::SetLeftThumbStick(BtU32 index, MtVector2 v2Position )
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	joystickInstance.m_isUsed = BtTrue;
	joystickInstance.m_v2LeftThumbStick = v2Position;
}

////////////////////////////////////////////////////////////////////////////////
// SetRightThumbStick

void ShJoystick::SetRightThumbStick(BtU32 index, MtVector2 v2Position )
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	joystickInstance.m_isUsed = BtTrue;
	joystickInstance.m_v2RightThumbStick = v2Position;
}

////////////////////////////////////////////////////////////////////////////////
// GetLeftThumbStick

MtVector2 ShJoystick::GetLeftThumbStick(BtU32 index)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	return joystickInstance.m_v2LeftThumbStick;
}

////////////////////////////////////////////////////////////////////////////////
// GetRightThumbStick

MtVector2 ShJoystick::GetRightThumbStick( BtU32 index )
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	return joystickInstance.m_v2RightThumbStick;
}

////////////////////////////////////////////////////////////////////////////////
// GetRightThumbStick

void ShJoystick::SetTransform(BtU32 index, MtMatrix4 &m4Transform)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	joystickInstance.m_m4Transform = m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// GetTransform

MtMatrix4 ShJoystick::GetTransform(BtU32 index)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	MtMatrix4 &m4Transform = joystickInstance.m_m4Transform;
	return m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

MtVector3 ShJoystick::GetPosition(BtU32 index)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	MtMatrix4 &m4Transform = joystickInstance.m_m4Transform;
	return m4Transform.GetTranslation();
}

////////////////////////////////////////////////////////////////////////////////
// SetTrigger

//static
void ShJoystick::ShJoystick::SetTrigger(BtU32 index, BtFloat trigger)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	joystickInstance.m_trigger = trigger;
}

////////////////////////////////////////////////////////////////////////////////
// GetTrigger

//static
BtFloat ShJoystick::GetTrigger(BtU32 index)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	return joystickInstance.m_trigger;
}

////////////////////////////////////////////////////////////////////////////////
// SetHeld

void ShJoystick::SetHeld(BtU32 index, ShJoystickButton button, BtBool isHeld)
{
	ShJoystickInstance &joystickInstance = m_joystick[index];
	joystickInstance.m_isHeld[button] = isHeld;
}