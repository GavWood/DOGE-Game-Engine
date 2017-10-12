////////////////////////////////////////////////////////////////////////////////
// CgCard3.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard3.h"
#include "SbMain.h"
#include "RsUtil.h"
#include "HlDraw.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard3::Setup()
{
	HlView::Setup("card3", "card3", BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard3::OnEntry()
{
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard3::OnExit()
{
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard3::Update()
{
	HlView::Update();

	// Cache each item
	HlMenuItems &item = m_items[0];

	if( item.m_isReleased )
	{
		SbMain::SetNextState();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard3::Render()
{
	HlView::Render();

	BtU32 sortOrder = MaxSortOrders - 1;

	MtVector2 v2Half = RsUtil::GetDimension() * 0.5f;
	MtVector2 v2Small = RsUtil::GetDimension() / 3.0f;
	RsColour col(RsColour::BlackColour());

	HlDraw::SetCrossColour(RsColour::RedColour(), RsColour::GreenColour(), RsColour::BlueColour());

	HlDraw::RenderLine(v2Half + MtVector2(-v2Small.x, -v2Small.y), v2Half + MtVector2( v2Small.x, -v2Small.y), col, sortOrder);
	HlDraw::RenderLine(v2Half + MtVector2(-v2Small.x,  v2Small.y), v2Half + MtVector2( v2Small.x,  v2Small.y), col, sortOrder);
	HlDraw::RenderLine(v2Half + MtVector2(-v2Small.x, -v2Small.y), v2Half + MtVector2(-v2Small.x,  v2Small.y), col, sortOrder);
	HlDraw::RenderLine(v2Half + MtVector2( v2Small.x, -v2Small.y), v2Half + MtVector2( v2Small.x,  v2Small.y), col, sortOrder);

	RsColour col2(1.0f, 0.8f, 0.6f, 1.0f);
	MtVector2 v2Smaller = MtVector2( v2Small.x, v2Small.y) * 0.5f; 
	HlDraw::RenderQuad( v2Half - v2Small, v2Smaller, col2, sortOrder);

	MtVector2 v2Start = v2Half - v2Small + MtVector2(v2Smaller.x * 0.5f , 0);
	MtVector2 v2End = v2Start + MtVector2(0, v2Small.y);
	HlDraw::RenderCross(v2End, v2Half.x * 0.1f, MaxSortOrders - 1);
}
