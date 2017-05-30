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
#include "corona.h"

#include <map>
#include <iterator>

enum class GameState m_currentState;
enum class GameState m_nextState;

struct SbOtherPlayerState
{
	GameState m_state;
};

// Special behavior for ++GameState
GameState& operator++(GameState &c)
{
	int iv = static_cast<int>(c);
	if (iv < static_cast<int>( GameState::GameState_MAX ))
	{
		iv++;
	}
	c = static_cast<GameState>(iv);
	return c;
}

// Special behavior for GameState++
GameState operator++(GameState &c, int) {
	GameState result = c;
	++c;
	return c;
}

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
		ApConfig::SetResourcePath("..\\SynchronisedPlay\\release\\");
		ApConfig::CheckResourcePath("utility");
	}
	else if (ApConfig::GetDevice() == ApDevice_iPhone )
	{
		ApConfig::CheckResourcePath("utility");
	}

	m_peers.Setup();
	MpPeerToPeer::Create();

	m_currentState = GameState::GameState_MAX;
	m_nextState = GameState::GameState_ShowCard0;
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

	m_pShader = m_utilityArchive.GetShader( "shader" );

	m_card.Setup( &m_utilityArchive );

	HlFont::Setup( &m_utilityArchive, "vera" );

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

std::map<MpPeer, SbOtherPlayerState, cmpNetworkPeer> networkAllStates;

void SbMain::PullNetworkChanges()
{
	MpPeerToPeer::Update();
	m_peers.Update();

	for (BtU32 i = 0; i < MpPeerToPeer::GetNumEvents(); i++)
	{
		MpEvent peerEvent = MpPeerToPeer::PeekEvent(i);
		if (peerEvent.m_eventType == MpEventType_ReceivedData)
		{
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
				SbOtherPlayerState playersState = *((SbOtherPlayerState*)packet->GetData());

				if (networkAllStates.insert(std::make_pair(peerEvent.m_peer, playersState)).second == false)
				{
					networkAllStates[peerEvent.m_peer] = playersState;
				}
			}
		}
	}

	// Add this computers game state to the pool so we can compare against everyones
	SbOtherPlayerState other;
	other.m_state = m_nextState;
	if (networkAllStates.insert( std::make_pair( MpPeerToPeer::GetHost(), other )).second == false)
	{
		networkAllStates[ MpPeerToPeer::GetHost() ] = other;
	}

	// This is now a map of all the network states
	std::map< MpPeer, SbOtherPlayerState>::iterator it = networkAllStates.begin();

	// Loop through it and find out what the highest recorded state is
	GameState currentState = GameState::GameState_ShowCard0;
	while (it != networkAllStates.end())
	{
		// Get another player state
		SbOtherPlayerState playerState = it->second;

		// Is it higher than the one we've seen
		currentState = MtMax(currentState, playerState.m_state);

		// Increment the pointer
		it++;
	}

	if( currentState != m_nextState )
	{
		m_nextState = currentState;
	}
}

////////////////////////////////////////////////////////////////////////////////
// PushNetworkChanges

void SbMain::PushNetworkChanges()
{
	// Send this to the other peers - at the moment this is just which game state we are on
	SbGamePacket packet(SbGamePacketType_ShareState);
	SbOtherPlayerState nextState;
	nextState.m_state = m_nextState;
	packet >> nextState;
	packet.SendReliable();
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateTests()
{
	HlDebug::Reset();
	HlDebug::Update();

	// Update the camera
	m_camera.Update();

	// Get what everyone else is doing
	PullNetworkChanges();

	// Check if the state has changed
	if( m_currentState != m_nextState )
	{
		// Calculate the card index
		BtU32 cardIndex = (BtU32) m_nextState - (BtU32) GameState::GameState_ShowCard0;

		// Load the new card
		m_card.LoadCard(cardIndex);

		// Push the changes to all other clients
		PushNetworkChanges();
	}

	// Set the next state
	m_currentState = m_nextState;

	// This is where the state machine gets executed
	int a = 0;
	a++;
	switch (m_currentState)
	{
	case GameState::GameState_ShowCard0:
		break;
	}

	// Look for key presses
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_ESCAPE))
	{
		m_nextState = (GameState)0;
	}
	if(UiKeyboard::pInstance()->IsPressed(DebugKey))
	{
		ApConfig::SetDebug(!ApConfig::IsDebug());
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_1))
	{
		SbMain::SetNextState( GameState::GameState_ShowCard0 );
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_2))
	{
		SbMain::SetNextState( GameState::GameState_ShowCard1 );
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_3))
	{
		SbMain::SetNextState( GameState::GameState_ShowCard2 );
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_4))
	{
		SbMain::SetNextState( GameState::GameState_ShowCard3 );
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
		m_timer.ResetTimer(0.5f);
	}
	if (m_timer.IsReady())
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
	return m_nextState;
}

////////////////////////////////////////////////////////////////////////////////
// SetNextState

void SbMain::SetNextState()
{
	m_nextState++;
}

////////////////////////////////////////////////////////////////////////////////
// SetNextState

void SbMain::SetNextState(GameState state)
{
	m_nextState = state;
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
		UpdateTests();
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
