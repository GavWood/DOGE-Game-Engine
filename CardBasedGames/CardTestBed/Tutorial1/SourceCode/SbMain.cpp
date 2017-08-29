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
#include "MtMath.h"

BtU32 SbMain::m_nextState;
BtU32 SbMain::m_currentState;
const BtU32 MaxCard = 6;

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	ApConfig::Init();
	ApConfig::SetTitle( "Card based games tutorial" );
	ApConfig::SetDebug( BtFalse );
	RdRandom::SetRandomSeed();

	// Load the game archive
	if (ApConfig::GetDevice() == ApDevice_WIN)
	{
		ApConfig::SetResourcePath("..\\tutorial1\\release\\");
		ApConfig::CheckResourcePath("utility");
	}
	else if (ApConfig::GetDevice() == ApDevice_iPhone )
	{
		ApConfig::CheckResourcePath("utility");
	}

	m_peers.Setup();
	MpPeerToPeer::Create();

	m_currentState = 5;
	m_nextState = m_currentState;
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

	m_card1.Setup();
	m_card2.Setup();
	m_card3.Setup();
	m_card4.Setup( &m_utilityArchive );
	m_card5.Setup();
	m_card6.Setup();
	m_card7.Setup();

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
// UpdateTest

void SbMain::UpdateTests()
{
	HlDebug::Reset();
	
	if (UiKeyboard::pInstance()->IsPressed(ShutdownPeersKey))
	{
		SbGamePacket packet(SbGamePacketType_PeerShutdown);
		packet.SendReliable();
		m_isClosing = BtTrue;
	}
	
	MpPeerToPeer::Update();
	m_peers.Update();
	m_camera.Update();

	// Set the next state
	m_currentState = m_nextState;

	switch (m_currentState)
	{
		case 0:
			m_card1.Update();
			break;

		case 1:
			m_card2.Update();
			break;

		case 2:
			m_card3.Update();
			break;

		case 3:
			m_card4.Update();
			break;

		case 4:
			m_card5.Update();
			break;

		case 5:
			m_card6.Update();
			break;

		case 6:
			m_card7.Update();
			break;
	}

	// Shutdown code to close all peers
	for( BtU32 i = 0; i < MpPeerToPeer::GetNumEvents(); i++)
	{
		MpEvent peer = MpPeerToPeer::PeekEvent(i);
		if (peer.m_eventType == MpEventType_ReceivedData)
		{
			SbGamePacket *packet = (SbGamePacket*)peer.m_pData;
			if (packet->GetType() == SbGamePacketType_PeerShutdown )
			{
				m_isClosing = BtTrue;
			}
		}
	}
	
	HlDebug::Update();

	if(UiKeyboard::pInstance()->IsPressed(DebugKey))
	{
		ApConfig::SetDebug(!ApConfig::IsDebug());
	}

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_ESCAPE))
	{
		SbMain::SetNextState(0);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_PERIOD))
	{
		BtU32 state = SbMain::GetState();
		SbMain::SetNextState( MtMin( (BtU32)(state + 1), MaxCard ));
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_COMMA))
	{
		BtU32 state = SbMain::GetState();
		if (state > 0)
		{
			SbMain::SetNextState( state - 1 );
		}
	}

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_1))
	{
		SbMain::SetNextState(0);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_2))
	{
		SbMain::SetNextState(1);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_3))
	{
		SbMain::SetNextState(2);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_4))
	{
		SbMain::SetNextState(3);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_5))
	{
		SbMain::SetNextState(4);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_6))
	{
		SbMain::SetNextState(5);
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_7))
	{
		SbMain::SetNextState(6);
	}

	if (UiKeyboard::pInstance()->IsPressed(CloseGameKey))
	{
		m_isClosing = BtTrue;
	}

	if (UiKeyboard::pInstance()->IsPressed(PauseKey))
	{
		ApConfig::SetPaused( !ApConfig::IsPaused() );
	}

	// Last thing we do in the main loop is to remove the 
	MpPeerToPeer::DeletePackets();
}

////////////////////////////////////////////////////////////////////////////////
// SetNextState

void SbMain::SetNextState()
{
	m_nextState++;
	if (m_nextState > MaxCard)
	{
		m_nextState = MaxCard;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetState

BtU32 SbMain::GetState()
{
	return m_currentState;
}

////////////////////////////////////////////////////////////////////////////////
// SetNextState

void SbMain::SetNextState(BtU32 state)
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

	// Update the camera
	m_camera.Render();

	switch (m_currentState)
	{
	case 0:
		m_card1.Render();
		break;

	case 1:
		m_card2.Render();
		break;

	case 2:
		m_card3.Render();
		break;

	case 3:
		m_card4.Render();
		break;

	case 4:
		m_card5.Render();
		break;

	case 5:
		{
			MtVector3 v3LightDirection( -1.0f, 0.25f, 0 );
			v3LightDirection.Normalise();
			m_pShader->SetDirectionalLight(v3LightDirection);
			m_pShader->SetAmbient(RsColour(0.3f, 0.3f, 0.3f, 0));
			m_card6.Render();
		}
		break;

	case 6:
		m_card7.Render();
		break;
	}

	// Render the mouse
	HlMouse::Render();

	HlDebug::Render();

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
