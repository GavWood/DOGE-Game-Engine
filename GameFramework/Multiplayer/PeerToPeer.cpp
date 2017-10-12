////////////////////////////////////////////////////////////////////////////////
// PeerToPeer.cpp

// Includes
#include "PeerToPeer.h"
#include "RsUtil.h"
#include "ApConfig.h"
#include "ErrorLog.h"
#include "HlFont.h"
#include <mutex>
#include <string>
#include "BtMemory.h"

#ifdef WIN32
#include "enetwrapper.h"
#endif

//static
BtQueue<MpEvent, 128> MpPeerToPeer::m_events;
BtQueue<MpEvent, 128> MpPeerToPeer::m_sendQueue;
std::set<MpPeer, MpPeerComp> MpPeerToPeer::m_peers;
BtU64 MpPeerToPeer::m_timeStarted = 0;
MpPeer MpPeerToPeer::m_host;

////////////////////////////////////////////////////////////////////////////////
// statics

//static

#ifdef WIN32
UDPNetwork udpNetwork;
#endif

////////////////////////////////////////////////////////////////////////////////
// Create

//static
void MpPeerToPeer::Create()
{
    m_sendQueue.Empty();
	m_events.Empty();
    
    // Get the time we started
	time_t now = time(0);
	m_timeStarted = now;

#ifdef WIN32
	BtU32 numInstances = ApConfig::GetNumInstances();
	udpNetwork.Create(numInstances, ApConfig::GetTitle(), m_timeStarted);
#endif
	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

//static
void MpPeerToPeer::Destroy()
{
#ifdef WIN32
	udpNetwork.Destroy();
#endif
}

////////////////////////////////////////////////////////////////////////////////
// GetTimeStarted

//static
BtU64 MpPeerToPeer::GetTimeStarted()
{
    return m_timeStarted;
}

////////////////////////////////////////////////////////////////////////////
// Update

//static
void MpPeerToPeer::Update()
{
#ifdef WIN32
	// Update the underlying UDP network
	udpNetwork.Update();

	// If there is a network event we need to add it to the cross platform list of network events
	if( udpNetwork.IsEvent() )
	{
		// Cache the udp event
		ENETEvent udpEvent = udpNetwork.PopEvent();

		// Cache the multi-player event
		MpEvent event;
		switch (udpEvent.m_eventType)
		{
		case UDPEventType_ReceivedData:
			event.m_eventType = MpEventType_ReceivedData;
			break;
		case UDPEventType_ClientConnected:
			event.m_eventType = MpEventType_ClientConnected;
			break;
		case UDPEventType_ClientDisconnected:
			event.m_eventType = MpEventType_ClientDisconnected;
			break;
		defaukt:
			BtAssert(0);
			break;
		}
		event.m_pData = udpEvent.data;
		event.m_length = udpEvent.length;
		event.m_dataForDelloc = udpEvent.m_underlying;
		event.m_peer.m_address = udpEvent.m_address.host;
		event.m_peer.m_port = udpEvent.m_address.port;
		strcpy(event.m_peer.m_networkName, udpEvent.m_networkHost );
		m_events.Push(event);
	}

	// Delete the peers
	while (!m_peers.empty())
	{
		m_peers.erase(m_peers.begin());
	}

	// Make a copy of the peers that is also cross platform
	BtU32 numPeers = udpNetwork.GetNumPeers();
	if (numPeers)
	{
		for (BtU32 i = 0; i < numPeers; i++)
		{
			// Get the UDP client and maintain our network
			UDPClient client = udpNetwork.GetPeer(i);

			// Add the new peer?
			MpPeer peer;
			peer.m_address = client.m_address.host;
			peer.m_port = client.m_address.port;
			peer.m_startTime = client.m_startTime;
			strcpy(peer.m_networkName, client.m_networkName);
			m_peers.insert(peer);
		}
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Disconnect

//static
void MpPeerToPeer::Disconnect()
{
#ifdef WIN32
	udpNetwork.Disconnect();
#endif
}

////////////////////////////////////////////////////////////////////////////////
// DeleteSendPacket

void MpPeerToPeer::DeleteSendPacket( MpEvent multiplayerEvent)
{
    BtMemory::Free( multiplayerEvent.m_pData );
}

////////////////////////////////////////////////////////////////////////////////
// DeletePacket

void MpPeerToPeer::DeletePacket( MpEvent multiplayerEvent)
{
#ifdef WIN32
	udpNetwork.DeletePacket(multiplayerEvent.m_dataForDelloc );
#endif
}

////////////////////////////////////////////////////////////////////////////////
// DeletePackets

//static
void MpPeerToPeer::DeletePackets()
{
	// Clean up the packets
	BtBool isEvent = MpPeerToPeer::IsEvent();
	while( isEvent )
	{
		// Cache the event and pop it off the stack
		MpEvent networkEvent = MpPeerToPeer::PopEvent();

		// If we received data delete the packet
		if( networkEvent.m_eventType == MpEventType_ReceivedData )
		{
			MpPeerToPeer::DeletePacket(networkEvent);
		}

		// Are there still events
		isEvent = MpPeerToPeer::IsEvent();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

//static
void MpPeerToPeer::Render()
{
	//BtChar *text = udpNetwork.GetText();
	//HlFont::Render(MtVector2(0, 0), 1.0f, text, RsColour::WhiteColour(), MaxSortOrders - 1);
}

////////////////////////////////////////////////////////////////////////////////
// IsOldestPeer

//static
BtBool MpPeerToPeer::IsFirstPeer()
{
	std::set<MpPeer, MpPeerComp>::iterator itt;

    if( ApConfig::IsWin() )
    {
        for( itt = m_peers.begin();  itt != m_peers.end(); ++itt )
        {
            if (m_timeStarted > itt->m_startTime)
            {
                return BtFalse;
            }
        }
    }
    else
    {
        for( itt = m_peers.begin();  itt != m_peers.end(); ++itt )
        {
            if ( strcmp( m_host.m_networkName, itt->m_networkName ) > 0 )
            {
                return BtFalse;
            }
        }
    }
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// GetPeer

//static
MpPeer MpPeerToPeer::GetHost( BtU32 index )
{
	// Cache the number of peers
	BtU32 numPeers = (BtU32)m_peers.size();

	// Validate this index is ok
	BtAssert(index < numPeers );

	// Return the peer at this position
	MpPeer peer = *std::next( m_peers.begin(), index );
	return peer;
}

////////////////////////////////////////////////////////////////////////////////
// GetHost

//static
MpPeer MpPeerToPeer::GetHost()
{
#ifdef WIN32
	ENetAddress host = udpNetwork.GetHost();
	m_host.m_address = host.host;
	m_host.m_port = host.port;
	return m_host;
#else
    int a=0;
    a++;
#endif
	// Return the peer at this position
	return m_host;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumPeers

BtU32 MpPeerToPeer::GetNumPeers()
{
	BtU32 numPeers = (BtU32)m_peers.size();
	return numPeers;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumEvents

BtU32 MpPeerToPeer::GetNumEvents()
{
	return m_events.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// SendUnsequenced

//static
void MpPeerToPeer::SendUnsequenced( void *packet, int length )
{
#ifdef WIN32
	udpNetwork.SendUnsequenced( packet, length );
#else
    BtU8 *temp = BtMemory::Allocate(length);
    memcpy( temp, packet, length );
    MpEvent event;
    event.m_pData = (BtChar*)temp;
    event.m_length = length;
    event.m_eventType = MpEventType_SendReliableData;
    m_sendQueue.Push(event);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// SendFragment

//static
void MpPeerToPeer::SendFragment(void *packet, int length)
{
#ifdef WIN32
	udpNetwork.SendFragment(packet, length);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// SendReliable

//static
void MpPeerToPeer::SendReliable(void *packet, int length)
{
#ifdef WIN32
	udpNetwork.SendReliable(packet, length);
#else
    BtU8 *temp = BtMemory::Allocate(length);
    memcpy( temp, packet, length );
    MpEvent event;
    event.m_pData = (BtChar*)temp;
    event.m_length = length;
    event.m_eventType = MpEventType_SendReliableData;
    m_sendQueue.Push(event);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// IsEvent

//static
BtBool MpPeerToPeer::IsEvent()
{
	return m_events.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopEvent

//static
MpEvent MpPeerToPeer::PopEvent()
{
	return m_events.Pop();
}

////////////////////////////////////////////////////////////////////////////////
// PeekEvent

//static
MpEvent MpPeerToPeer::PeekEvent( BtU32 index )
{
	return m_events.Peek(index);
}

////////////////////////////////////////////////////////////////////////////////
// PeekEvent

//static
void MpPeerToPeer::ClearPeers()
{
    m_peers.clear();
}

////////////////////////////////////////////////////////////////////////////////
// AddEvent

void MpPeerToPeer::AddEvent(MpEvent event)
{
    m_events.Push(event);
}

////////////////////////////////////////////////////////////////////////////////
// AddSendEvent

void MpPeerToPeer::AddToSendQueue(MpEvent event)
{
    m_sendQueue.Push(event);
}

////////////////////////////////////////////////////////////////////////////////
// SetHost

//static
void MpPeerToPeer::SetHost(MpPeer peer)
{
    m_host = peer;
}

////////////////////////////////////////////////////////////////////////////////
// AddPeer

//static
void MpPeerToPeer::AddPeer(MpPeer peer)
{
    // Fill in the time we started
    peer.m_startTime = m_timeStarted;
    m_peers.insert(peer);
}

////////////////////////////////////////////////////////////////////////////////
// DeletePeer

//static
void MpPeerToPeer::DeletePeer(MpPeer peer)
{
    // Fill in the time we started
    m_peers.erase(peer);
}

////////////////////////////////////////////////////////////////////////////////
// GetNumSendEvents

//static
BtU32 MpPeerToPeer::GetNumSendQueue()
{
    return m_sendQueue.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopSendEvent

//static
MpEvent	MpPeerToPeer::PopSendQueue()
{
    return m_sendQueue.Pop();
}
