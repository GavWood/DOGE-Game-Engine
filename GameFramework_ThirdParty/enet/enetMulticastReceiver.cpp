#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#include <set>
#include "enetMulticastReceiver.h"

struct sockaddr_in enetMulticastReceiveAddr;

void enetMulticastReceiver::Create(ENetAddress addressToSend, const char *game )
{	
	struct ip_mreq mreq;

	u_int yes = 1;            /*** MODIFICATION TO ORIGINAL */
							  /* create what looks like an ordinary UDP socket */
	if ((m_recvSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
	}

	/**** MODIFICATION TO ORIGINAL */
	/* allow multiple sockets to use the same PORT number */
	if (setsockopt(m_recvSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) < 0)
	{
		perror("Reusing ADDR failed");
	}
	/*** END OF MODIFICATION TO ORIGINAL */

	/* set up destination address */
	memset(&enetMulticastReceiveAddr, 0, sizeof(enetMulticastReceiveAddr));
	enetMulticastReceiveAddr.sin_family = AF_INET;
	enetMulticastReceiveAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B.: differs from sender */
	enetMulticastReceiveAddr.sin_port = htons(enetMulticastPort);

	/* bind to receive address */
	if (bind(m_recvSocket, (struct sockaddr *) &enetMulticastReceiveAddr, sizeof(enetMulticastReceiveAddr)) < 0)
	{
		perror("bind");
	}

	/* use setsockopt() to request that the kernel join a multicast group */
	mreq.imr_multiaddr.s_addr = inet_addr(enetMulticastAddress);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(m_recvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt");
	}
	if (setsockopt(m_recvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt");
	}

	u_long nonBlocking = (u_long)1;
	ioctlsocket(m_recvSocket, FIONBIO, &nonBlocking);

	m_begin = (float)clock();
	m_addressToSend = addressToSend;
	strcpy(m_gameTitle, game);
}

void enetMulticastReceiver::Update()
{
	// Delete the multicasters
	m_multicasters.clear();

	MulticastingGame multicastingGame;

	// Look for a new multicast broadcast
	clock_t current = clock();
	int addrlen = sizeof(enetMulticastReceiveAddr);
	int nbytes = recvfrom(m_recvSocket, (char*)&multicastingGame, sizeof(MulticastingGame), 0, (struct sockaddr *) &enetMulticastReceiveAddr, &addrlen);

	if( nbytes < 0 )
	{
		perror("recvfrom");
	}
	else if( nbytes > 0 )
	{
		// If it's this machine then don't do anything
		if( multicastingGame.IsEqual(m_addressToSend) )
		{
			int dontJointItsUs;
			dontJointItsUs = 0;
		}
		else
		{
			if (strcmp(m_gameTitle, multicastingGame.m_gameName) == 0 )
			{
				// Insert it again
				m_multicasters.push_back(multicastingGame);
			}
			else
			{
				// Not the right game
				int a = 0;
				a++;
			}
		}
	}
}

void enetMulticastReceiver::Destroy()
{
	closesocket(m_recvSocket);
}
