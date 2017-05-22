////////////////////////////////////////////////////////////////////////////////
// CgCard2.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard2.h"
#include "SbMain.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard2::Setup()
{
	HlView::Setup("card2", "card2", BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard2::OnEntry()
{
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard2::OnExit()
{
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard2::Update()
{
	HlView::Update();

	BtU32 numItems = m_items.GetNumItems();
	BtAssert(numItems > 0);

	// Cache each item
	HlMenuItems &touchPress = m_items[0];

	if (touchPress.m_isPressed)
	{
		SbMain::SetNextState();
	}

	HlMenuItems &touchUp = m_items[1];

	if (touchUp.m_isReleased)
	{
		SbMain::SetNextState();
	}

	HlMenuItems &touchHold = m_items[2];

	if(touchHold.m_isPressed || (GetCurrentItemSelected() != &touchHold))
	{
		// This resets the timer if the button isn't being held
		m_timer.ResetTimer(5.0f);
	}
	else if (touchHold.m_isHeld && ( GetCurrentItemSelected() == &touchHold ) )
	{
		if (m_timer.IsReady())
		{
			SbMain::SetNextState();
		}
		if (m_timer.GetElapsedTime() >= 5)
		{
			SbMain::SetNextState();
		}
	}
	else if( touchUp.m_isReleased )
	{
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard2::Render()
{
	HlView::Render();
}
