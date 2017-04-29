////////////////////////////////////////////////////////////////////////////////
// HlMaterial.h

// Include guard
#pragma once

#include "BtTypes.h"
#include "MtVector2.h"

class RsMaterial;

class HlGesture
{
public:

	HlGesture();

	void						Update();

	// Accessors
	BtBool						IsSwipeLeft();
	BtBool						IsSwipeRight();

private:

	MtVector2					m_v2Start;
	BtBool						m_isSwipeLeft;
	BtBool						m_isSwipeRight;
	BtBool						m_isMeasuring;
};
