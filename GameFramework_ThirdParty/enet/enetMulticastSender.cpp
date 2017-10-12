#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#include <set>
#include "enetMulticastSender.h"
#include "enetMulticastReceiver.h"
#include <time.h>

void enetMulticastSender::Create( ENetAddress addressToSend, const char *game, uint64_t timeStarted )
{	
	/* create what looks like an ordinary UDP socket */
	if( ( m_sendSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
	}

	m_addressToSend = addressToSend;
	m_timeClientStarted = timeStarted;
	strcpy(m_gameTitle, game);
	gethostname(m_hostName, sizeof(m_hostName));

	int a = 0;
	a++;
}

void enetMulticastSender::Update()
{
	static int s = 0;
	s--;
	if (s < 0)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(enetMulticastAddress);
		addr.sin_port = htons(enetMulticastPort);

		MulticastingGame multicastingGame;
		multicastingGame.m_address = m_addressToSend;
		multicastingGame.m_timeStarted = m_timeClientStarted;
		strcpy( multicastingGame.m_gameName, m_gameTitle );

		// Make the network name
		sprintf( multicastingGame.m_networkName, "%s:%d", m_hostName, m_addressToSend.port );
		int messageLen = sizeof(ENetAddress);

		int nbytes = sendto(m_sendSocket, (const char*)&multicastingGame, sizeof(MulticastingGame), 0, (struct sockaddr *) &addr, sizeof(addr));
		if (nbytes < 0)
		{
			perror("sendto");
		}
		else if (nbytes > 0)
		{
			int a = 0;
			a++;
		}

		s = 60 * 3;
	}
}

void enetMulticastSender::Destroy()
{
}
