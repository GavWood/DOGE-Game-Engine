////////////////////////////////////////////////////////////////////////////////
// SbPlayers.cpp

// Includes
#include "BaArchive.h"
#include "SbPeers.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsSprite.h"
#include "HlDebug.h"
#include "PeerToPeer.h"
#include "UiKeyboard.h"
#include "HlKeyboard.h"
#include "SbPeers.h"
#include "BtCrc.h"

////////////////////////////////////////////////////////////////////////////////
// GetType

SbGamePacketType SbGamePacket::GetType()
{
	return m_header.m_type;
}

////////////////////////////////////////////////////////////////////////////////
// GetLength

BtU32 SbGamePacket::GetLength()
{
	return m_length;
}

////////////////////////////////////////////////////////////////////////////////
// ValidateHashcode

void SbGamePacket::ValidateHashcode()
{
	BtU32 hashcode = BtCRC::GenerateHashCode((const BtU8*)m_data, m_length);
	BtAssert( m_header.m_hashcode == hashcode);
}

////////////////////////////////////////////////////////////////////////////////
// SendReliable

void SbGamePacket::SendReliable()
{
	m_header.m_hashcode = BtCRC::GenerateHashCode((const BtU8*)m_data, m_length);
	m_current = 0;
	MpPeerToPeer::SendReliable( this, sizeof(SbGamePacketHeader) + m_length );
}

////////////////////////////////////////////////////////////////////////////////
// SendUnsequenced

void SbGamePacket::SendUnsequenced()
{
	m_header.m_hashcode = BtCRC::GenerateHashCode( (const BtU8*)m_data, m_length );
	m_current = 0;
	MpPeerToPeer::SendUnsequenced( this, sizeof(SbGamePacketHeader) + m_length );
}

////////////////////////////////////////////////////////////////////////////////
// SendFragment

void SbGamePacket::SendFragment()
{
	BtAssert(m_length < 576);
	m_header.m_hashcode = BtCRC::GenerateHashCode((const BtU8*)m_data, m_length);
	m_current = 0;
	BtU32 headerSize = sizeof(SbGamePacketHeader);
	MpPeerToPeer::SendFragment(this, sizeof(SbGamePacketHeader) + m_length );
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbPeers::Setup()
{
	strcpy(m_status, "Client_started");
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbPeers::UpdateDebug()
{
	// Was there a network event
	BtU32 numEvents = MpPeerToPeer::GetNumEvents();
	for (BtU32 i = 0; i < numEvents; i++)
	{
		// Cache the event
		MpEvent networkEvent = MpPeerToPeer::PeekEvent(i);

		if (networkEvent.m_eventType == MpEventType_ClientConnected)
		{
			strcpy(m_status, "connected");
		}
		else if (networkEvent.m_eventType == MpEventType_ClientDisconnected)
		{
			strcpy(m_status, "disconnected");
		}
		else if (networkEvent.m_eventType == MpEventType_ReceivedData)
		{
			strcpy(m_status, "received_data");

			// Do something with packet here
			(void)(networkEvent);		// enough info to do some stuff
		}
	}

	m_numPlayers = MpPeerToPeer::GetNumPeers();
	m_isOldest = MpPeerToPeer::IsFirstPeer();

	// Add our debug stuff
	HlDebug::AddNode(BtNull, m_status);
	HlDebug::AddUInt(BtNull, "Num_peers", &m_numPlayers, BtTrue);
	HlDebug::AddBool(BtNull, "Is_oldest", &m_isOldest, BtTrue);
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbPeers::Update()
{
	UpdateDebug();

	// Scratch code for easy access to peers
	for ( BtU32 i = 0; i < m_numPlayers; i++ )
	{
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbPeers::Render()
{
}
