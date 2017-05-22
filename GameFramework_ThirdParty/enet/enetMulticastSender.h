#include "enet\enet.h"
#include <conio.h>
#include <set>

class enetMulticastSender
{
protected:

public:

	void						Create( ENetAddress addressToSend, const char *game, uint64_t timeStarted );
	void						Destroy();
	void						Update();

private:

	int							m_sendSocket;
	ENetAddress					m_addressToSend;
	uint64_t					m_timeClientStarted;
	char					    m_gameTitle[32];
	char					    m_hostName[64];
};
