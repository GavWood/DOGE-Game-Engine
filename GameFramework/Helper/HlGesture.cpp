////////////////////////////////////////////////////////////////////////////////
// HlGesture.cpp

#include "HlGesture.h"
#include "ShTouch.h"
#include "RsUtil.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

HlGesture::HlGesture()
{
	m_isSwipeLeft = BtFalse;
	m_isSwipeRight = BtFalse;
	m_isMeasuring = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlGesture::Update()
{
	m_isSwipeLeft = BtFalse;
	m_isSwipeRight = BtFalse;

	if ( ShTouch::IsPressed(0) )
	{
		m_v2Start = ShTouch::GetPosition(0);
		m_isMeasuring = BtTrue;
	}
	
	if (ShTouch::IsHeld(0) && m_isMeasuring )
	{
		MtVector2 v2Current = ShTouch::GetPosition(0);

		MtVector2 v2Delta = v2Current - m_v2Start;

		BtFloat width = RsUtil::GetWidth() * 0.25f;

		if( v2Delta.x < -width ) 
		{
			m_isSwipeLeft = BtTrue;
			m_isMeasuring = BtFalse;
		}
		else if (v2Delta.x > width)
		{
			m_isSwipeRight = BtTrue;
			m_isMeasuring = BtFalse;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsSwipeLeft

BtBool HlGesture::IsSwipeLeft()
{
	return m_isSwipeLeft;
}

////////////////////////////////////////////////////////////////////////////////
// IsSwipeRight

BtBool HlGesture::IsSwipeRight()
{
	return m_isSwipeRight;
}
