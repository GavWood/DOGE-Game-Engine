////////////////////////////////////////////////////////////////////////////////
// CgCard7.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard6.h"
#include "UiKeyboard.h"
#include "SbMain.h"
#include "MtMatrix4.h"
#include "SgNode.h"
#include "HlModel.h"
#include "BtTime.h"
#include "FFT.h"
#include "SdSound.h"
#include "BtQueue.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard7::Setup()
{
	HlView::Setup("card7", "card7", BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard7::OnEntry()
{
	SdSound::StartCapture(BtFalse);
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard7::OnExit()
{
	SdSound::StopCapture();
}

BtFloat myPitch = 0;
BtFloat myVolume = 0;

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard7::Update()
{
	HlView::Update();

	// Cache each item
	HlMenuItems &item = m_items[0];

	if (item.m_isReleased)
	{
		SbMain::SetNextState(0);
	}

    // Update the sound
    SdSound::UpdateCapture();
    
    // Cache the pitch and volume
	BtFloat pitch = SdSound::GetPitch();
	BtFloat vol = SdSound::GetVolume();

    // Sample the volume
	m_amplitude.Push(vol);

	BtFloat avg = 0;

	for (BtU32 i = 0; i < m_amplitude.GetItemCount(); i++)
	{
		avg += m_amplitude.Peek(i);
	}
	avg /= (BtFloat)m_amplitude.GetItemCount();

	if (m_amplitude.GetItemCount() == m_amplitude.GetMaxItems() )
	{
		m_amplitude.Pop();
	}

    myVolume = avg;
    
    // Sample the pitch
    m_pitch.Push(pitch);
    
    avg = 0;
    
    for (BtU32 i = 0; i < m_pitch.GetItemCount(); i++)
    {
        avg += m_pitch.Peek(i);
    }
    avg /= (BtFloat)m_pitch.GetItemCount();
    
    if (m_pitch.GetItemCount() == m_pitch.GetMaxItems() )
    {
        m_pitch.Pop();
    }
    
    myPitch = avg;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard7::Render()
{
	HlView::Render();
	BtChar buffer[64];

	sprintf(buffer, "Pitch %.2f\nVolume %.0f", myPitch, myVolume );
	HlFont::Render(MtVector2(10, 20), 1.0f, buffer, RsColour::BlackColour(), MaxSortOrders - 1);
}
