////////////////////////////////////////////////////////////////////////////////
// ShNetwork.cpp

// Includes
#include "ShNetwork.h"
#include "RsUtil.h"
#include <mutex>

////////////////////////////////////////////////////////////////////////////////
// statics

//static

// Public functions
BtQueue<ShNetworkAction, 128> ShNetwork::m_actions;
BtQueue<ShNetworkAction, 128> ShNetwork::m_alerts;
BtU32 ShNetwork::m_numConnectedPeers = 0;
std::vector<std::string> ShNetwork::m_connectedPeers;
std::string ShNetwork::m_peerName;
std::mutex networkMutex;

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShNetwork::PushAction( ShNetworkAction action )
{
    m_actions.Push( action );
}

////////////////////////////////////////////////////////////////////////////////
// PushAlert

//static
void ShNetwork::PushAlert( ShNetworkAction action )
{
    m_alerts.Push( action );
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

//static
BtU32 ShNetwork::GetNumItems()
{
    return m_actions.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// GetNumAlerts

//static
BtU32 ShNetwork::GetNumAlerts()
{
    return m_alerts.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// GetNumConnectedPeers

//static
BtU32 ShNetwork::GetNumConnectedPeers()
{
    return m_numConnectedPeers;
}

////////////////////////////////////////////////////////////////////////////////
// GetName

std::string ShNetwork::GetName( BtU32 index )
{
    return m_connectedPeers[index];
}

////////////////////////////////////////////////////////////////////////////////
// GetPeerName

std::string ShNetwork::GetPeerName()
{
    return m_peerName;
}

////////////////////////////////////////////////////////////////////////////////
// SetPeerName

void ShNetwork::SetPeerName( std::string peerName )
{
    m_peerName = peerName;
}

////////////////////////////////////////////////////////////////////////////////
// SetConnectedPeers

void ShNetwork::SetConnectedPeers( std::vector<std::string> connectedPeers )
{
    std::lock_guard<std::mutex> lock(networkMutex);
    m_connectedPeers = connectedPeers;
    m_numConnectedPeers = (int)connectedPeers.size();
}

////////////////////////////////////////////////////////////////////////////////
// PopAlert

//static
ShNetworkAction ShNetwork::PopAlert()
{
    ShNetworkAction action;
    action = m_alerts.Pop();
    return action;
}

////////////////////////////////////////////////////////////////////////////////
// PopAction

//static
ShNetworkAction ShNetwork::PopAction()
{
    ShNetworkAction action;
    action = m_actions.Pop();
    return action;
}


