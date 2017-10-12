////////////////////////////////////////////////////////////////////////////////
// ShKeyboard

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "MtVector2.h"

const BtFloat TriggerDeadZone = 0.1f;

enum ShJoystickButton
{
	JoystickButton_Pause,
	JoystickButton_Y,
	JoystickButton_B,
	JoystickButton_A,
	JoystickButton_X,
    JoystickButton_LeftShoulder,
	JoystickButton_RightShoulder,
    JoystickButton_Up,
	JoystickButton_Down,
	JoystickButton_Left,
	JoystickButton_Right,
    JoystickButton_Menu,
    JoystickButton_Pressed,
	JoystickButton_Trigger,
	JoystickButton_MAX,
};

const BtU32 MaxJoysticks = 8;

struct ShJoystickInstance
{
	BtBool								    m_isConnected;
	BtBool									m_isUsed;

	MtVector2								m_v2LeftThumbStick;
	MtVector2								m_v2RightThumbStick;

	BtBool									m_isHeld[JoystickButton_MAX];
	BtBool									m_isReleased[JoystickButton_MAX];
	BtBool									m_isPressed[JoystickButton_MAX];
	BtBool									m_lastPressed[JoystickButton_MAX];

	MtMatrix4								m_m4Transform;

	BtFloat									m_trigger;
};

// Class definition
class ShJoystick
{
public:
    
    ShJoystick();

	static void								Update();

    static void                             BeginTouch( BtU32 m_joystickIndex, ShJoystickButton button );
    static void                             EndTouch( BtU32 m_joystickIndex, ShJoystickButton button );
    
	static BtBool							IsHeld( BtU32 m_joystickIndex, ShJoystickButton button );
    static BtBool							IsPressed( BtU32 m_joystickIndex);
    static BtBool							IsPressed(BtU32 m_joystickIndex, ShJoystickButton button );
	static BtBool							IsReleased( BtU32 m_joystickIndex, ShJoystickButton button );
    
    static BtBool                           IsButtonPressed( BtU32 m_joystickIndex);
    static BtBool                           IsDirectionPressed( BtU32 m_joystickIndex);
    
    static void                             SetLeftThumbStick( BtU32 m_joystickIndex, MtVector2 v2Position );
    static void                             SetRightThumbStick( BtU32 m_joystickIndex, MtVector2 v2Position );
	static MtVector2						GetLeftThumbStick( BtU32 m_joystickIndex);
	static MtVector2						GetRightThumbStick( BtU32 m_joystickIndex);

	static void		                        SetTrigger(BtU32 m_joystickIndex, BtFloat trigger);
	static BtFloat	                        GetTrigger(BtU32 m_joystickIndex);

	// Accessors
    static BtBool							IsConnected( BtU32 m_joystickIndex);
    static BtBool							IsUsed( BtU32 m_joystickIndex);
    static void                             SetUsed( BtU32 m_joystickIndex, BtBool isUsed );
    static void                             SetConnected( BtU32 m_joystickIndex, BtBool isConnected );

	static void                             SetTransform(BtU32 m_joystickIndex, MtMatrix4 &m4Transform );
	static MtMatrix4                        GetTransform(BtU32 m_joystickIndex);
	static MtVector3                        GetPosition(BtU32 m_joystickIndex);

	static void 							SetHeld(BtU32 m_joystickIndex, ShJoystickButton button, BtBool isHeld);

private:

	static ShJoystickInstance				m_joysticks[MaxJoysticks];
};
