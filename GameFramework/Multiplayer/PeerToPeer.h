////////////////////////////////////////////////////////////////////////////////
// MpPeerToPeer

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtQueue.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"
#include <set>
#include "BtMemory.h"

const BtU32 MaxNetworkQueue = 128;

enum MpEventType
{
    MpEventType_SendReliableData,
    MpEventType_SendUnsequencedData,
    MpEventType_ReceivedData,
	MpEventType_ClientConnected,
	MpEventType_ClientDisconnected,
};

struct MpPeer
{
	bool operator == (const MpPeer&& rhs)const
	{
        #ifdef WIN32
		if( (m_address == rhs.m_address) && (m_port == rhs.m_port) )
			return true;
        #else
        if( strcmp( m_networkName, rhs.m_networkName ) == 0 )
        {
            return true;
        }
        #endif
		return false;
	}

	BtU32										m_address;
	BtU32										m_port;
	BtU64										m_startTime;
    BtChar                                      m_networkName[64];
};

struct MpEvent
{
	MpPeer										m_peer;					// Who are they?
	MpEventType									m_eventType;
	BtChar									   *m_pData;
	BtU32										m_length;
	void									   *m_dataForDelloc;
};

struct MpPeerComp
{
	bool operator() (const MpPeer& lhs, const MpPeer& rhs) const
	{
#ifdef WIN32
		if (lhs.m_address < rhs.m_address )
			return true;
		if (lhs.m_port < rhs.m_port)
			return true;
#else
        if( strcmp( lhs.m_networkName, rhs.m_networkName ) < 0 )
        {
            return true;
        }
#endif
		return false;
	}
};

// Class definition
class MpPeerToPeer
{
public:

	// Access
    static BtU64                                GetTimeStarted();
	static void									Create();
	static void									Update();
	static void									Render();
	static void									SendReliable( void *packet, int length );
	static void									SendUnsequenced(void *packet, int length);
	static void									SendFragment(void *packet, int length);
	static void									Destroy();
	static void									DeletePackets();
	static void									Disconnect();

	// Accessors
	static BtU32								GetNumPeers();
	static MpPeer								GetHost(BtU32 index);
	static MpPeer								GetHost();

	// Events
	static BtU32								GetNumEvents();
	static BtBool								IsEvent();
	static MpEvent								PopEvent();
	static MpEvent								PeekEvent( BtU32 index );
	static BtBool								IsFirstPeer();
    
	
	// Functionality to hook in with other network providers
	static void                                 AddEvent(MpEvent event);
	static void                                 ClearPeers();
    static void                                 AddPeer(MpPeer peer);
    static void                                 DeletePeer(MpPeer peer);
    static void                                 SetHost(MpPeer peer);

	// Send queue
    static void                                 AddToSendQueue( MpEvent event );
    static BtU32								GetNumSendQueue();
    static MpEvent								PopSendQueue();
    static void									DeleteSendPacket(MpEvent event);
    
private:

	// Private functions
    static void									DeletePacket(MpEvent event);
	
	// Member variables
	static std::set<MpPeer, MpPeerComp>			m_peers;
	static BtQueue<MpEvent, 128>				m_events;
	static BtQueue<MpEvent, 128>				m_receiveEvents;
    static BtQueue<MpEvent, 128>				m_sendQueue;
	static BtU64								m_timeStarted;
    static MpPeer                               m_host;
};
