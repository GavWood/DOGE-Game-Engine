////////////////////////////////////////////////////////////////////////////////
// CgCard5.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard5.h"
#include "UiKeyboard.h"
#include "SbMain.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard5::Setup()
{
	HlView::Setup("card5", "card5", BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard5::OnEntry()
{
	m_pSound = m_archive.GetSound("howl");
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard5::OnExit()
{
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard5::Update()
{
	HlView::Update();

	// Cache each item
	HlMenuItems &item = m_items[0];

	if (item.m_isPressed)
	{
		m_pSound->Play();
	}

	if (m_items[1].m_isReleased )
	{
		SbMain::SetNextState();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard5::Render()
{
	HlView::Render();
}
