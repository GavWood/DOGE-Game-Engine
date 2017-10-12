////////////////////////////////////////////////////////////////////////////////
// ShTouch.h

#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "RsCamera.h"

const BtU32 WMTouches = 8;
const BtU32 MaxTouches = 8 + 8;

// Class definition
class ShTouch
{
public:

	// Constructor
	ShTouch();

	// State functions

	// Public functions
	static void					BeginTouch(BtU32 index, const MtVector2& v2Position);
	static void					MoveTouch(BtU32 index, const MtVector2& v2Position);
	static void					EndTouch(BtU32 index, const MtVector2& v2Position);
	static void                 EndShake();

	static void					SetAlive( BtU32 index );
    static BtBool				IsAlive( BtU32 index );

	static void					Update();

	static MtVector3			GetPositionFromTouch(RsCamera camera, MtVector2 v2Position, BtFloat distance);
	
	// Accessors	
	static BtFloat				DurationHeld(BtU32 index);
	static BtBool				IsHeld(BtU32 index);
	static BtBool				IsReleased(BtU32 index);
	static BtBool				IsPressed(BtU32 index);
   
    // All keys
    static BtBool               IsAction( BtU32 touch );
    static BtBool				IsPressed();
    static BtBool				IsHeld();
    static BtBool				IsReleased();
	
	static const MtVector2&		GetPosition( BtU32 touch );
	static const MtVector2&		GetMovement( BtU32 touch );
	static BtBool				OnScreen(BtU32 index);

	static BtU32				GetNumberOfTaps();
	static void					SetNumberOfTaps( BtU32 numTaps );

	static BtU32				GetNumberOfTouches();
    static BtU32				GetNumberOfTouchesReleased();
    static BtU32				GetMaxNumberOfTouches();

	static void                 SetShaken();
	static BtBool               IsShaken();

private:

	static void					SetPosition( BtU32 index, const MtVector2& v2Position );

	// Private members
	static MtVector2			m_v2Movement[MaxTouches];
	static MtVector2			m_v2Position[MaxTouches];
	static MtVector2			m_v2LastPosition[MaxTouches];
	static BtBool				m_isHeld[MaxTouches];
	static BtFloat				m_durationHeld[MaxTouches];
	static BtBool				m_isReleased[MaxTouches];
	static BtBool				m_isFirstTime[MaxTouches];
	static BtBool				m_isPressed[MaxTouches];
	static BtBool				m_isAlive[MaxTouches];
    static BtBool               m_isShaken;
	static BtBool				m_lastPressed[MaxTouches];
	static BtU32				m_numberOfTaps;
	static BtU32				m_maxNumberOfTouches;
	static BtU32				m_maxNumberOfTouchesReleased;
};
