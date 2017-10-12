////////////////////////////////////////////////////////////////////////////////
// ShTouch.cpp

// Includes
#include "ShTouch.h"
#include "RsUtil.h"
#include "BtMemory.h"
#include "BtTime.h"

// Statics
ShTouch touch;

BtBool ShTouch::m_isFirstTime[MaxTouches];
MtVector2 ShTouch::m_v2Movement[MaxTouches];
MtVector2 ShTouch::m_v2Position[MaxTouches];
MtVector2 ShTouch::m_v2LastPosition[MaxTouches];
BtBool ShTouch::m_isHeld[MaxTouches];
BtFloat ShTouch::m_durationHeld[MaxTouches];
BtBool ShTouch::m_isReleased[MaxTouches];
BtBool ShTouch::m_isPressed[MaxTouches];
BtBool ShTouch::m_isAlive[MaxTouches];
BtBool ShTouch:: m_isShaken;
BtBool ShTouch::m_lastPressed[MaxTouches];
BtU32 ShTouch::m_numberOfTaps;
BtU32 ShTouch::m_maxNumberOfTouches;
BtU32 ShTouch::m_maxNumberOfTouchesReleased;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ShTouch::ShTouch()
{
	for( BtU32 index = 0; index < MaxTouches; index++ )
	{
		m_lastPressed[index] = BtFalse;
		m_isHeld[index]      = BtFalse;
		m_isReleased[index]  = BtFalse;
		m_v2Position[index]  = MtVector2( 0, 0 );
		m_isAlive[index]     = BtFalse;
		m_isFirstTime[index] = BtTrue;
	}
	m_numberOfTaps = 0;
    m_maxNumberOfTouches = 0;
	m_maxNumberOfTouchesReleased = 0;
    m_isShaken = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ShTouch::Update()
{
	m_maxNumberOfTouchesReleased = 0;

	for( BtU32 index = 0; index < MaxTouches; index++ )
	{
		m_isPressed[index] = BtFalse;
		m_isReleased[index] = BtFalse;

		if( m_isHeld[index] == BtTrue )
		{
			m_durationHeld[index] += BtTime::GetTick();

			if( m_lastPressed[index] == BtFalse )
			{
				m_isPressed[index] = BtTrue;
			}
			m_lastPressed[index] = BtTrue;
		}
		else
		{
			m_durationHeld[index] = 0;

			if( m_lastPressed[index] == BtTrue )
			{
				m_isReleased[index] = BtTrue;

				m_maxNumberOfTouchesReleased = m_maxNumberOfTouches;
			}
			m_lastPressed[index] = BtFalse;
		}
	}
    
    m_isShaken = BtFalse;

	// Set the number of touches
	BtU32 currentTouches = GetNumberOfTouches();

	if( currentTouches == 0 )
	{
		m_maxNumberOfTouches = 0;
	}
	else
	{
		m_maxNumberOfTouches = MtMax( m_maxNumberOfTouches, currentTouches );
	}
}

////////////////////////////////////////////////////////////////////////////////
// BeginTouch

void ShTouch::BeginTouch( BtU32 index, const MtVector2& v2Position )
{
    m_v2Position[index] = v2Position;
	
    SetPosition( index, v2Position );

	m_isHeld[index] = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// SetAlive

void ShTouch::SetAlive( BtU32 index )
{
	m_isAlive[index] = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// IsAlive

BtBool ShTouch::IsAlive( BtU32 index ) 
{
	return m_isAlive[index];
}

////////////////////////////////////////////////////////////////////////////////
// MoveTouch

void ShTouch::MoveTouch( BtU32 index, const MtVector2& v2Position )
{
    SetPosition( index, v2Position );
}

////////////////////////////////////////////////////////////////////////////////
// EndTouch

void ShTouch::EndTouch( BtU32 index, const MtVector2& v2Position )
{
    m_v2Position[index] = v2Position;

    ShTouch::SetPosition( index, v2Position );

	m_isHeld[index] = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// DurationHeld

BtFloat ShTouch::DurationHeld( BtU32 index )
{
	return m_durationHeld[index];
}

////////////////////////////////////////////////////////////////////////////////
// IsHeld

BtBool ShTouch::IsHeld( BtU32 index )
{
	return m_isHeld[index];
}

////////////////////////////////////////////////////////////////////////////////
// IsReleased

BtBool ShTouch::IsReleased( BtU32 index )
{
    if( m_isReleased[index] == BtTrue )
    {
        int a=0;
        a++;
    }
	return m_isReleased[index];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

BtBool ShTouch::IsPressed( BtU32 index )
{
	return m_isPressed[index];
}

////////////////////////////////////////////////////////////////////////////////
// IsAction

BtBool ShTouch::IsAction( BtU32 touch )
{
    return IsPressed(touch) || IsHeld(touch) || IsReleased(touch);
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

BtBool ShTouch::IsPressed()
{
    for( BtU32 i=0; i<MaxTouches; i++ )
    {
        if( m_isPressed[i] == BtTrue )
        {
            return BtTrue;
        }
    }
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsHeld

BtBool ShTouch::IsHeld()
{
    for( BtU32 i=0; i<MaxTouches; i++ )
    {
        if( m_isHeld[i] == BtTrue )
        {
            return BtTrue;
        }
    }
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsReleased

BtBool ShTouch::IsReleased()
{
    for( BtU32 i=0; i<MaxTouches; i++ )
    {
        if( m_isReleased[i] == BtTrue )
        {
            return BtTrue;
        }
    }
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// OnScreen

BtBool ShTouch::OnScreen(BtU32 touch)
{
	MtVector2 v2Position = m_v2Position[touch];
	if( v2Position.x < 0 )
	{
		return BtFalse;
	}
	if (v2Position.y < 0 )
	{
		return BtFalse;
	}
	if (v2Position.x > RsUtil::GetWidth() )
	{
		return BtFalse;
	}
	if (v2Position.y > RsUtil::GetHeight() )
	{
		return BtFalse;
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

const MtVector2& ShTouch::GetPosition( BtU32 touch )
{
	return m_v2Position[touch];
}

////////////////////////////////////////////////////////////////////////////////
// GetMovement

const MtVector2& ShTouch::GetMovement( BtU32 touch )
{
	return m_v2Movement[touch];
}

////////////////////////////////////////////////////////////////////////////////
// SetPosition

void ShTouch::SetPosition( BtU32 index, const MtVector2& v2Position )
{
	if( m_isFirstTime[index] )
	{
		m_v2LastPosition[index] = v2Position;
		m_isFirstTime[index] = BtFalse;
	}
	else
	{
		m_v2LastPosition[index] = m_v2Position[index];
	}

	// Copy the positions
	m_v2Position[index] = v2Position;

	// Calculate the movement
	m_v2Movement[index] = v2Position - m_v2LastPosition[index];	
}

////////////////////////////////////////////////////////////////////////////////
// SetNumberOfTaps

void ShTouch::SetNumberOfTaps( BtU32 numTaps )
{
	m_numberOfTaps = numTaps;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumberOfTaps

BtU32 ShTouch::GetNumberOfTaps()
{
	return m_numberOfTaps;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaxNumberOfTouches

BtU32 ShTouch::GetMaxNumberOfTouches()
{
	return m_maxNumberOfTouchesReleased;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumberOfTouchesReleased

BtU32 ShTouch::GetNumberOfTouchesReleased()
{
    BtU32 numTouches = 0;
    
    for( BtU32 index = 0; index < MaxTouches; index++ )
    {
        if( m_isReleased[index] == BtTrue )
        {
            ++numTouches;
        }
    }
    return numTouches;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumberOfTouches

BtU32 ShTouch::GetNumberOfTouches()
{
	BtU32 numTouches = 0;
	
	for( BtU32 index = 0; index < MaxTouches; index++ )
	{
		if( m_isHeld[index] == BtTrue )
		{
			++numTouches;
		}
	}
	return numTouches;
}

////////////////////////////////////////////////////////////////////////////////
// SetShaken

void ShTouch::SetShaken()
{
    m_isShaken = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// IsShaken

BtBool ShTouch::IsShaken()
{
    return m_isShaken;
}

////////////////////////////////////////////////////////////////////////////////
// GetPositionFromTouch

MtVector3 ShTouch::GetPositionFromTouch(RsCamera camera, MtVector2 v2Position, BtFloat distance)
{
	BtFloat aspect = RsUtil::GetAspect();
	BtFloat fieldOfView = camera.GetFieldOfView() * 0.5f;

	BtFloat x = v2Position.x;
	BtFloat y = v2Position.y;

	x = x / (BtFloat)RsUtil::GetWidth();
	y = y / (BtFloat)RsUtil::GetHeight();

	y = 1.0f - y;

	x = x - 0.5f;
	y = y - 0.5f;

	x = x * 2.0f;
	y = y * 2.0f;

	x = x * MtTan(fieldOfView) * distance * aspect;
	y = y * MtTan(fieldOfView) * distance;

	MtMatrix3 m3Rotation = camera.GetRotation().GetInverse();

	MtVector3 v3AxisX = m3Rotation.XAxis();
	MtVector3 v3AxisY = m3Rotation.YAxis();
	MtVector3 v3AxisZ = m3Rotation.ZAxis();

	v3AxisX = v3AxisX * x;
	v3AxisY = v3AxisY * y;

	MtVector3 v3Position = camera.GetPosition();
	v3Position += v3AxisX + v3AxisY + (v3AxisZ * distance);
	return v3Position;
}
