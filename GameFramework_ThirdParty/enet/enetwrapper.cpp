#include "enet\enet.h"
#include <conio.h>
#include <set>
#include <assert.h>
#include "enetwrapper.h"
#include "process.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
// Create

void UDPNetwork::Create( int instanceIndex, const char *title, uint64_t timeStarted )
{	
	if (enet_initialize())
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return;
	}

	char ac[80];
	char ip[80];
	strcpy(ip, "");
	if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR)
	{
		struct hostent *phe = gethostbyname(ac);
		if (phe != 0)
		{
			for (int i = 0; phe->h_addr_list[i] != 0; ++i)
			{
				struct in_addr addr;
				memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
				char *temp = inet_ntoa(addr);
				strcat(ip, temp);
			}
		}
	}

	// Create the host
	int portIndex = 12345 + instanceIndex;

	enet_address_set_host(&m_hostAddress, ip );	// localhost address
	
	m_hostAddress.port = portIndex;

	m_host = enet_host_create( &m_hostAddress /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		0      /* assume any amount of incoming bandwidth */,
		0,     /* assume any amount of outgoing bandwidth */
		0);
	if (!m_host)
	{
		printf("An error occurred while trying to create an ENet server host.\n");
		return;
	}

	m_multicastSend.Create(m_hostAddress, title, timeStarted );
	m_multicastReceiver.Create(m_hostAddress, title );

	m_isLooking = true;

	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// GetHost

ENetAddress UDPNetwork::GetHost()
{
	return m_hostAddress;
}

////////////////////////////////////////////////////////////////////////////////
// SendReliable

void UDPNetwork::SendReliable(void *packet, int length )
{
	ENetPacket* enetPacket = enet_packet_create( packet, length, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast( m_host, 0, enetPacket );
}

////////////////////////////////////////////////////////////////////////////////
// SendFragment

void UDPNetwork::SendFragment(void *packet, int length)
{
	// MTU size
	assert(length < 576);
	ENetPacket* enetPacket = enet_packet_create(packet, length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
	enet_host_broadcast(m_host, 0, enetPacket);
}

////////////////////////////////////////////////////////////////////////////////
// SendUnsequenced

void UDPNetwork::SendUnsequenced(void *packet, int length)
{
	ENetPacket* enetPacket = enet_packet_create(packet, length, ENET_PACKET_FLAG_UNSEQUENCED);
	enet_host_broadcast(m_host, 0, enetPacket);
}

////////////////////////////////////////////////////////////////////////////////
// DeletePacket

void UDPNetwork::DeletePacket(void *packet)
{
	enet_packet_destroy( (ENetPacket*)packet);
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void UDPNetwork::Destroy()
{
	m_multicastReceiver.Destroy();
	m_multicastSend.Destroy();

	// Disconnect the clients
	std::set<UDPClient, UDPClientComp>::iterator it;
	for (it = m_joinedHosts.begin(); it != m_joinedHosts.end(); ++it)
	{
		enet_uint32 data = m_host->address.port;
		enet_peer_disconnect( it->m_pPeer, data);
	}

	// Clear the clients
	for (it = m_joinedHosts.begin(); it != m_joinedHosts.end(); ++it)
	{
		enet_host_destroy(it->m_client);
	}

	// Clear the host
	if (m_host)
	{
		enet_host_destroy(m_host);
	}
	enet_deinitialize();
}

////////////////////////////////////////////////////////////////////////////////
// IsEvent

bool UDPNetwork::IsEvent()
{
	if( m_events.size() > 0 )
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// PopEvent

ENETEvent UDPNetwork::PopEvent()
{
	ENETEvent event = m_events.top();
	m_events.pop();
	return event;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void UDPNetwork::Update()
{
	if (m_isLooking)
	{
		m_multicastSend.Update();
		m_multicastReceiver.Update();
	}

	// Create a list of queued hosts 
	std::vector<MulticastingGame> multicasters = m_multicastReceiver.GetMulticasters();
	for ( int i=0; i<(int)multicasters.size(); i++ )
	{
		// Cache the multicast broadcaster that we want to join
		MulticastingGame hostToJoin = multicasters[i];

		// Look for this host in our list of joined hosts
		UDPClient conn;
		conn.m_address = hostToJoin.m_address;

		// Find this host
		std::set< UDPClient, UDPClientComp >::iterator itt = m_joinedHosts.find(conn);

		// Add this to the list to join
		bool isJoined = itt != m_joinedHosts.end();

		if (isJoined)
		{
			// erase the old one and rejoin
			if (hostToJoin.m_timeStarted != itt->m_startTime)
			{
				m_joinedHosts.erase(itt);
			}
		}

		if (!isJoined)
		{
			// Create a new client and try to join it to the address
			UDPClient clientConn;
			clientConn.m_startTime = hostToJoin.m_timeStarted;
			clientConn.m_address = hostToJoin.m_address;
			clientConn.m_client = enet_host_create(NULL /* create a client host */,
				2 /* only allow 1 outgoing connection */,
				4 /* allow up 2 channels to be used, 0 and 1 */,
				0 /* 56K modem with 56 Kbps downstream bandwidth */,
				0 /* 56K modem with 14 Kbps upstream bandwidth */);

			// Copy the network name
			strcpy((char*)clientConn.m_networkName, (char*)hostToJoin.m_networkName);

			// Try and connect
			clientConn.m_pPeer = enet_host_connect(clientConn.m_client, &clientConn.m_address, 2, 0 /* two channels*/);

			// Add this for connection
			m_joinedHosts.insert(clientConn);
		}
	}
	
	// Looking for events where we a client has connected to us
	if (m_host && enet_host_service(m_host, &m_hostEvent, 1) > 0)
	{
		if (m_hostEvent.type == ENET_EVENT_TYPE_DISCONNECT)
		{
			// Find where we are connected to this host in our list. We have to have a small hack here
			// by supplying the port number with the disconnect information so we can find it in our list
			UDPClient conn;
			conn.m_address = m_hostEvent.peer->address;
			conn.m_address.port = m_hostEvent.data;
			std::set< UDPClient, UDPClientComp >::iterator itt = m_joinedHosts.find(conn);

			// Add this to the list to join
			if ( itt != m_joinedHosts.end() )
			{
				ENETEvent enetEvent;
				enetEvent.m_address = conn.m_address;
				enetEvent.m_eventType = UDPEventType_ClientDisconnected;
				enetEvent.data = 0;
				enetEvent.length = 0;
				m_events.push(enetEvent);

				// Remove this client
				enet_host_destroy(itt->m_client);
				m_joinedHosts.erase(itt);
			}
		}
	}

	// Looking for events where we are connected as a client to a host
	std::set<UDPClient, UDPClientComp>::iterator it;
	for( it = m_joinedHosts.begin(); it != m_joinedHosts.end(); ++it)
	{
		ENETEvent eevent;
		if (enet_host_service(it->m_client, &eevent.mClientEvent, 1) > 0)
		{
			if ( eevent.mClientEvent.type == ENET_EVENT_TYPE_CONNECT)
			{
				eevent.m_address = it->m_address;
				eevent.m_eventType = UDPEventType_ClientConnected;
				strcpy(eevent.m_networkHost, it->m_networkName);
				m_events.push(eevent);
			}
			if (eevent.mClientEvent.type == ENET_EVENT_TYPE_RECEIVE)
			{
				eevent.m_address = it->m_address;
				eevent.m_eventType = UDPEventType_ReceivedData;
				eevent.data = (char*)eevent.mClientEvent.packet->data;
				eevent.length = (size_t)eevent.mClientEvent.packet->dataLength;
				eevent.m_underlying = (void*)eevent.mClientEvent.packet;
				strcpy(eevent.m_networkHost, it->m_networkName);
				m_events.push(eevent);
				
				int a = 0;
				a++;
			}
			if (eevent.mClientEvent.type == ENET_EVENT_TYPE_DISCONNECT)
			{
				eevent.m_address = it->m_address;
				eevent.m_eventType = UDPEventType_ClientDisconnected;
				strcpy(eevent.m_networkHost, it->m_networkName);
				m_events.push(eevent);

				// Remove this client
				enet_host_destroy(it->m_client);
				m_joinedHosts.erase(it);
				break;
			}
		}
	}

	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Disconnect

void UDPNetwork::Disconnect()
{
	// Stop broadcasting that we are interested in connecting to anyone
	m_isLooking = false;

	// Disconnect all the clients from any hosts we are connected to
	std::set<UDPClient, UDPClientComp>::iterator it;
	for (it = m_joinedHosts.begin(); it != m_joinedHosts.end(); ++it)
	{
		enet_uint32 data = m_host->address.port;
		enet_peer_disconnect(it->m_pPeer, data);
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNumPeers

int UDPNetwork::GetNumPeers()
{
	return m_joinedHosts.size();
}

////////////////////////////////////////////////////////////////////////////////
// GetPeer

UDPClient UDPNetwork::GetPeer(int index)
{
	UDPClient client = *std::next( m_joinedHosts.begin(), index );
	return client;
}