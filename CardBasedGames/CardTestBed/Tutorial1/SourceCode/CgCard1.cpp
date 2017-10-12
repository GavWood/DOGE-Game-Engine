////////////////////////////////////////////////////////////////////////////////
// CgCard1.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard1.h"
#include "UiKeyboard.h"
#include "SbMain.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard1::Setup()
{
	HlView::Setup("card1", "card1", BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard1::OnEntry()
{
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard1::OnExit()
{
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard1::Update()
{
	HlView::Update();

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_SPACE))
	{
		SbMain::SetNextState();
	}
    
    // Cache each item
    HlMenuItems &touchPress = m_items[0];
    
    if (touchPress.m_isReleased)
    {
        SbMain::SetNextState();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard1::Render()
{
	HlView::Render();
}
