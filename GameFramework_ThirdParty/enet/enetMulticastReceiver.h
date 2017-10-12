#include "enet\enet.h"
#include <conio.h>
#include <vector>
#include <ctime>

#define enetMulticastPort 12340
#define enetMulticastAddress "225.0.0.37"

struct MulticastingGame
{
	char									    m_gameName[32];
	char										m_networkName[64];
	uint64_t									m_timeStarted;
	ENetAddress									m_address;

	bool IsEqual(ENetAddress address)
	{
		if ((m_address.host == address.host) && (m_address.port == address.port))
		{
			return true;
		}
		return false;
	}
};

class enetMulticastReceiver
{
protected:
	std::vector<MulticastingGame>				m_multicasters;

public:

	void										Create(ENetAddress addressToSend, const char *game );
	void										Destroy();

	void										Update();

	// Accessors
	std::vector<MulticastingGame>				GetMulticasters() { return m_multicasters; }

private:

	ENetAddress									m_addressToSend;
	int											m_recvSocket;
	float										m_begin;
	char									    m_gameTitle[32];
};
