////////////////////////////////////////////////////////////////////////////////
// CgCard4.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard4.h"
#include "SbMain.h"
#include "RsUtil.h"
#include "HlDraw.h"
#include "UiKeyboard.h"
#include "PeerToPeer.h"
#include "BaArchive.h"
#include "RsSprite.h"
#include "ShTouch.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard4::Setup( BaArchive *archive )
{
	HlView::Setup("card4", "card4", BtTrue);
	m_pArchive = archive;
	m_pSprite = m_pArchive->GetSprite("MousePointers");
	m_v2Position = MtVector2(0, 0);
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard4::OnEntry()
{
	// You can load archives here if you want
	//m_gameArchive.Load("game");
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard4::OnExit()
{
	//m_gameArchive.Unload();
	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard4::Update()
{
	HlView::Update();

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_P))
	{
		SbGamePacket packet(SbGamePacketType_Ping);
		packet.SendReliable();
	}

	if ( m_items[0].m_isPressed)
	{
		SbGamePacket packet(SbGamePacketType_Ping);
		packet.SendReliable();
	}
	
	if ( m_items[1].m_isReleased)
	{
		SbMain::SetNextState();
	}

	// Detect a ping
	for (BtU32 i = 0; i < MpPeerToPeer::GetNumEvents(); i++)
	{
		MpEvent peer = MpPeerToPeer::PeekEvent(i);
		if (peer.m_eventType == MpEventType_ReceivedData)
		{
			SbGamePacket *packet = (SbGamePacket*)peer.m_pData;
			if (packet->GetType() == SbGamePacketType_Ping)
			{
				m_pingCount++;
			}
			else if (packet->GetType() == SbGamePacketType_Mouse)
			{
				m_v2Position = *((MtVector2*)packet->GetData());
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard4::Render()
{
	HlView::Render();

    BtU32 numPeers = MpPeerToPeer::GetNumPeers();
	BtChar text[256];
	sprintf(text, "First peer %d\nPing %d\nNum peer %d", MpPeerToPeer::IsFirstPeer(), m_pingCount, numPeers );

	MtVector2 v2Position = RsUtil::GetDimension() / 10.0f;
	MtVector2 v2Dimension = HlFont::GetDimension(text);
	v2Position.x = v2Position.x - ( v2Dimension.x * 0.5f );
	HlFont::RenderHeavy(v2Position, text, MaxSortOrders - 1);

	RsColour colour(1.0f, 1.0f, 1.0f, 0.1f);
	m_pSprite->Render(BtFalse, m_v2Position, 1, 0, colour, MaxSortOrders - 1);
}
