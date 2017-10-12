////////////////////////////////////////////////////////////////////////////////
// SbMain.cpp

// Includes
#include <stdio.h>
#include "SbMain.h"
#include "ApConfig.h"
#include "RsUtil.h"
#include "RsShader.h"
#include "SbCamera.h"
#include "HlMaterial.h"
#include "HlFont.h"
#include "HlDraw.h"
#include "HlMouse.h"
#include "UiKeyboard.h"
#include "RsMaterial.h"
#include "ShHMD.h"
#include "UiKeyboard.h"
#include "HlKeyboard.h"
#include "ShTouch.h"
#include "PeerToPeer.h"
#include "HlDebug.h"
#include "SgNode.h"
#include "HlModel.h"

#include <map>
#include <iterator>

////////////////////////////////////////////////////////////////////////////////
// SbPlayerState
// This is the main game state which indicates who's turn it is and
// which card is being displayed

struct SbPlayerState
{
	SbPlayerState()
	{
		m_currentState = GameState::GameState_MAX;
		m_nextState = GameState::GameState_Ace;
		m_isTurn = BtFalse;
	}

	BtBool				m_isTurn;
	enum GameState		m_currentState;
	enum GameState		m_nextState;
	
	static GameState GetDefaultState()
	{
		GameState gameState;
		return gameState;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Overloaded operator++ for GameState (pre-increment is called last)

GameState& operator++(GameState &c)
{
	int iv = static_cast<int>(c);
	iv++;
	c = static_cast<GameState>(iv);
	return c;
}

////////////////////////////////////////////////////////////////////////////////
// Overloaded operator++ for GameState - (increment is called first)

GameState operator++(GameState &c, int) {
	GameState result = c;
	++c;
	return c;
}

SbPlayerState g_gameState;

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	ApConfig::Init();
	ApConfig::SetTitle( "Turn Taking Play Tutorial" );
	ApConfig::SetDebug( BtFalse );
	RdRandom::SetRandomSeed();

	// Load the game archive
	if (ApConfig::GetDevice() == ApDevice_WIN)
	{
		ApConfig::SetResourcePath("..\\TurnTakingPlay\\release\\");
		ApConfig::CheckResourcePath("utility");
	}
	else if (ApConfig::GetDevice() == ApDevice_iPhone )
	{
		ApConfig::CheckResourcePath("utility");
	}

	m_peers.Setup();
	MpPeerToPeer::Create();
}

////////////////////////////////////////////////////////////////////////////////
// Create

void SbMain::Create()
{
	m_utilityArchive.Load("utility");

	RsMaterial *pMaterial2 = m_utilityArchive.GetMaterial("white2");
	RsMaterial *pMaterial3 = m_utilityArchive.GetMaterial("white3");
	RsMaterial *pMaterial3notest = m_utilityArchive.GetMaterial("white3noztest");
	
	HlDraw::Setup(pMaterial2, pMaterial3, pMaterial3notest);
	HlFont::Setup(&m_utilityArchive, "vera");
	HlMouse::Setup(&m_utilityArchive);

	m_card.Setup( &m_utilityArchive );
	HlFont::Setup( &m_utilityArchive, "vera" );

	m_pShader = m_utilityArchive.GetShader("shader");

	MtVector2 v2Dimension = RsUtil::GetDimension();
	m_camera.Init(v2Dimension);
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbMain::Reset()
{
	MtVector2 v2Dimension = RsUtil::GetDimension();

	if (ShHMD::IsHMD())
	{
		v2Dimension = ShHMD::GetDimension();
	}

	m_camera.Init( v2Dimension );
	m_camera.SetPosition(MtVector3(0, 0, 0));
	m_camera.SetSpeed(10.0f);
}

////////////////////////////////////////////////////////////////////////////////
// PullNetworkChanges

struct cmpNetworkPeer
{
	bool operator()( const MpPeer& a, const MpPeer& b) const
	{
		return( strcmp( a.m_networkName, b.m_networkName ) < 0 );
	}
};

std::map<MpPeer, SbPlayerState, cmpNetworkPeer> networkAllStates;

void SbMain::PullNetworkChanges()
{
	MpPeerToPeer::Update();
	m_peers.Update();

	// If this is the first peer and we haven't iterated our card make it our turn
	if (MpPeerToPeer::IsFirstPeer() && ( g_gameState.m_currentState == GameState::GameState_Ace ) )
	{
		g_gameState.m_isTurn = BtTrue;
	}

	// Add us to the map so we can have some convenient game logic
	SbPlayerState state;
	networkAllStates[MpPeerToPeer::GetHost()] = state;

	// Look at the network events
	for (BtU32 i = 0; i < MpPeerToPeer::GetNumEvents(); i++)
	{
		MpEvent peerEvent = MpPeerToPeer::PeekEvent(i);
		
		if (peerEvent.m_eventType == MpEventType_ClientConnected)
		{
			// When a client connects - we must add it to our map of peers
			networkAllStates[peerEvent.m_peer] = state;
		}
		else if (peerEvent.m_eventType == MpEventType_ReceivedData)
		{
			// Respond to peer events
			SbGamePacket *packet = (SbGamePacket*)peerEvent.m_pData;
			if (packet->GetType() == SbGamePacketType_Ping)
			{
				static BtU32 m_pingCount = 0;
				m_pingCount++;
			}

			if (packet->GetType() == SbGamePacketType_PeerShutdown)
			{
				m_isClosing = BtTrue;
			}

			if (packet->GetType() == SbGamePacketType_ShareState)
			{
				// Find this peer
				std::map< MpPeer, SbPlayerState>::iterator it = networkAllStates.find(peerEvent.m_peer);

				// Get the next player
				it++;

				// If its the last player cycle round again
				if( it == networkAllStates.end() )
				{
					it = networkAllStates.begin();
				}

				// Make it our turn if its us
				if( it->first == MpPeerToPeer::GetHost() )
				{
					g_gameState.m_isTurn = BtTrue;
				}			
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// PushNetworkChanges

void SbMain::PushNetworkChanges()
{
	// Send this to the other peers - at the moment this is just which game state we are on
	SbGamePacket packet(SbGamePacketType_ShareState);
	packet >> g_gameState;
	packet.SendReliable();
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateTests()
{
	// Check if the state has changed
	if (g_gameState.m_currentState != g_gameState.m_nextState)
	{
		// Set the current state to the next state after the above check which responds to state change
		g_gameState.m_currentState = g_gameState.m_nextState;

		// Calculate the card index
		BtU32 cardIndex = (BtU32)g_gameState.m_currentState - (BtU32)GameState::GameState_Ace;

		// Load the new card
		m_card.LoadCard(cardIndex);

		// Push the changes to all other clients
		PushNetworkChanges();
	}

	HlDebug::AddBool(BtNull, "IsTurn", &g_gameState.m_isTurn, BtTrue);

	// Update the camera
	m_camera.Update();

	// Get what everyone else is doing
	PullNetworkChanges();

	// Look for key presses
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_ESCAPE))
	{
		g_gameState.m_nextState = (GameState)0;
	}
	if(UiKeyboard::pInstance()->IsPressed(DebugKey))
	{
		ApConfig::SetDebug(!ApConfig::IsDebug());
	}
	if (UiKeyboard::pInstance()->IsPressed(CloseGameKey))
	{
		m_isClosing = BtTrue;
	}
	if (UiKeyboard::pInstance()->IsPressed(PauseKey))
	{
		ApConfig::SetPaused( !ApConfig::IsPaused() );
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_F10))
	{
		SbGamePacket packet(SbGamePacketType_PeerShutdown);
		packet.SendReliable();
		m_closingTimer.ResetTimer(0.5f);
	}
	if( m_closingTimer.IsReady() )
	{
		SetClosing();
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_P))
	{
		SbGamePacket packet(SbGamePacketType_Ping);
		packet.SendReliable();
	}

	// Update the card
	m_card.Update();

	// Last thing we do in the main loop is to remove the peer to peer packets (IMPORTANT - OTHERWISE MEMORY LEAK)
	MpPeerToPeer::DeletePackets();
}

////////////////////////////////////////////////////////////////////////////////
// GetState

GameState SbMain::GetState()
{
	return g_gameState.m_nextState;
}

////////////////////////////////////////////////////////////////////////////////
// SetNextState

void SbMain::SetNextState()
{
	// Once we have our go - mark that our turn is over
	g_gameState.m_isTurn = BtFalse;

	// Increment the game state
	g_gameState.m_nextState++;

	// If we have reached the maximum state loop around again
	if (g_gameState.m_nextState == GameState::GameState_MAX)
	{
		g_gameState.m_nextState = GameState::GameState_Ace;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetNextState

void SbMain::SetNextState(GameState state)
{
	// Once we have our go - mark that our turn is over
	g_gameState.m_isTurn = BtFalse;

	// Set the new state
	g_gameState.m_nextState = state;
}

////////////////////////////////////////////////////////////////////////////////
// IsTurn

BtBool SbMain::IsTurn()
{
	return g_gameState.m_isTurn;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMain::Update()
{
	// Are we closing
	if( m_isClosing == BtTrue )
	{		
		// Unload the archive
		m_utilityArchive.Unload();

		// Read to close
		m_isClosed = BtTrue;
	}
	else
	{
		// Reset the debug info - we add new ones each time
		HlDebug::Reset();

		// Update the tests
		UpdateTests();

		// Update the debug as the last step
		HlDebug::Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// RestoreRenderTarget

void SbMain::RestoreRenderTarget()
{
	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( m_camera.GetCamera() );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour( 0.6f, 0.5f, 0.25, 0 ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// RenderTests

void SbMain::RenderTests()
{
	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();

	// Apply the shader
	m_pShader->Apply();

	// Restore the render target
	RestoreRenderTarget();

	// Render the card
	m_card.Render();

	// Render the debug info
	HlDebug::Render();

	// Render the mouse
	HlMouse::Render();	

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbMain::Render()
{
	if( m_isClosing == BtTrue )
	{
		return;
	}

	// Render the tests
	RenderTests();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbMain::Destroy()
{
	MpPeerToPeer::Destroy();
}
