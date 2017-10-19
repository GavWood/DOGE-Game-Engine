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

BtFloat pitch = 0;
BtFloat myVolume = 0;

const int ampSample = 128;
BtQueue<BtFloat, ampSample> amplitude;

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

	if( 1 )
	{
        SdSound::UpdateCapture();
        
		pitch = SdSound::GetPitch();
		BtFloat vol = SdSound::GetVolume();

		amplitude.Push(vol);

		BtFloat avg = 0;

		for (BtU32 i = 0; i < ampSample; i++)
		{
			avg = amplitude.Peek(i);
		}
		avg /= (BtFloat)ampSample;

		if (amplitude.GetItemCount() == ampSample)
		{
			amplitude.Pop();
		}

		myVolume = avg;
	}
	else
	{
		pitch = 0;
		myVolume = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard7::Render()
{
	HlView::Render();
	BtChar buffer[64];

	sprintf(buffer, "Pitch %.2f\nVolume %.0f", pitch, myVolume );
	HlFont::Render(MtVector2(100, 20), 1.0f, buffer, RsColour::BlackColour(), MaxSortOrders - 1);
}
