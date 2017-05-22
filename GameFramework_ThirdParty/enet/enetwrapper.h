#include "enet\enet.h"
#include "enetMulticastSender.h"
#include "enetMulticastReceiver.h"
#include <conio.h>
#include <set>
#include <stack>

enum UDPEventType
{
	UDPEventType_ReceivedData,
	UDPEventType_ClientConnected,
	UDPEventType_ClientDisconnected,
};

class ENETEvent
{
	public:

		ENETEvent()
		{
			data = 0;
			length = 0;
			m_underlying = 0;
		}

		ENetEvent								mClientEvent;
		ENetAddress								m_address;
		UDPEventType							m_eventType;
		char									m_networkHost[64];
		char								   *data;
		int										length;
		void								   *m_underlying;
};

struct UDPClient
{
	ENetAddress									m_address;
	ENetHost								   *m_client;
	ENetPeer								   *m_pPeer;
	uint64_t									m_startTime;
	char										m_networkName[64];

	bool IsEqual(ENetAddress address)
	{
		if( (m_address.host == address.host) && (m_address.port == address.port) )
		{
			return true;
		}
		return false;
	}
};

struct UDPClientComp
{
	bool operator() (const UDPClient& lhs, const UDPClient& rhs) const
	{
		if (lhs.m_address.host < rhs.m_address.host)
			return true;
		if (lhs.m_address.port < rhs.m_address.port)
			return true;
		return false;
	}
};

class UDPNetwork
{
public:

	void											Create(int instanceIndex, const char *title, uint64_t timeStarted );
	void											Destroy();

	void											Update();

	void											SendReliable(void *packet, int length);
	void											SendUnsequenced(void *packet, int length);
	void											SendFragment(void *packet, int length);

	void											Disconnect();
	void											DeletePacket(void *packet);

	// Accessors
	char										   *GetText();
	int												GetNumPeers();
	UDPClient										GetPeer(int index);
	bool											IsEvent();
	ENETEvent										PopEvent();
	ENetAddress										GetHost();

private:

	ENetHost									   *m_host;
	ENetEvent										m_hostEvent;
	enetMulticastReceiver							m_multicastReceiver;
	enetMulticastSender								m_multicastSend;
	ENetAddress										m_hostAddress;
	std::stack<ENETEvent>							m_events;
	std::set<UDPClient, UDPClientComp>				m_joinedHosts;
	bool											m_isLooking;
};
