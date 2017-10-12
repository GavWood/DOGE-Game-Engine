////////////////////////////////////////////////////////////////////////////////
// ShNetwork

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
#include <vector>
#include <string>

enum ShNetworkActionType
{
	ShNetworkActionType_Send,
    ShNetworkActionType_Received,
    ShNetworkActionType_DidChangeState,
};

struct ShNetworkAction
{
    ShNetworkActionType                     m_type;
    BtChar                                  m_peerID[64];
    BtChar                                  m_buffer[4096];
    BtU32                                   m_size;
};

// Class definition
class ShNetwork
{
public:

	// Access
	static void                             PushAction( ShNetworkAction action );
    static void                             PushAlert( ShNetworkAction action );
	static ShNetworkAction                  PopAction();
    static ShNetworkAction                  PopAlert();
    
    static BtU32                            GetNumItems();
    static BtU32                            GetNumAlerts();
    static BtU32                            GetNumConnectedPeers();
    
    static std::string                      GetPeerName();
    static std::string                      GetName( BtU32 index );
    
    static void                             SetPeerName( std::string peerName );
    static void                             SetConnectedPeers( std::vector<std::string> );
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShNetworkAction, 128>    m_actions;
    static BtQueue<ShNetworkAction, 128>    m_alerts;
    static BtU32                            m_numConnectedPeers;
    static std::vector<std::string>         m_connectedPeers;
    static std::string                      m_peerName;
};
