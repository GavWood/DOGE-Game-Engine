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
#include "HlDebug.h"
#include "SgNode.h"
#include "HlModel.h"
#include "BtQueue.h"

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	MtVector2 v2Dimension;

	ApConfig::Init();
	ApConfig::SetTitle( "Murmuration" );
	//ApConfig::SetDebug( BtTrue );
	ApConfig::SetDebug( BtFalse );
	RdRandom::SetRandomSeed();

	// Load the game archive
    if( ApConfig::IsWin() )
    {
        ApConfig::SetResourcePath( "..\\murmuration\\release\\" );
        ApConfig::CheckResourcePath( "game" );
    }
	RsUtil::SetDimension(MtVector2(800.0f, 600.0f));
}

////////////////////////////////////////////////////////////////////////////////
// Create

void SbMain::Create()
{
	m_gameArchive.Load( "game" );
	m_utilityArchive.Load("utility");

	m_pWhite2 = m_utilityArchive.GetMaterial( "white2" );
	m_pShader = m_gameArchive.GetShader( "shader" );
	 
	m_skybox.Setup( &m_gameArchive );
	m_model.Setup( &m_gameArchive, BtNull);
	
	RsMaterial *pMaterial2 = m_utilityArchive.GetMaterial("white2");
	RsMaterial *pMaterial3 = m_utilityArchive.GetMaterial("white3");
	RsMaterial *pMaterial3notest = m_utilityArchive.GetMaterial("white3noztest");
	m_pGUIRenderTarget = m_gameArchive.GetMaterial( "guirendertarget" );
	m_pGUIRenderTarget->SetTechniqueName( "RsShaderT" );

	HlFont::Setup( &m_utilityArchive, "vera" );
	HlMouse::Setup( &m_utilityArchive );
	HlDraw::Setup( pMaterial2, pMaterial3, pMaterial3notest );

	m_joysticks.Setup(pMaterial2, pMaterial3, pMaterial3notest );
	//m_joysticks.Setup(pNode1, pNode2);

	strcpy(m_text, "");

	MtVector2 v2Dimension = RsUtil::GetDimension();
	if (ShHMD::IsHMD())
	{
		v2Dimension = ShHMD::GetDimension();
	}
	m_camera.Init(v2Dimension);

	m_murmuration.Setup(&m_gameArchive);
    
    Reset();
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
	m_camera.SetSpeed(100.0f);

	m_murmuration.Reset();
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateTest()
{
	HlDebug::Reset();

	m_camera.Update();
	m_skybox.Update( m_camera.GetCamera().GetPosition() );
	m_model.Update();
	m_murmuration.Update();

	HlDebug::Update();

	//    m_cameraVideo.Update();

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
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMain::Update()
{
	// Are we closing
	if( m_isClosing == BtTrue )
	{
		// Unload the archive
		m_gameArchive.Unload();
	
		// Unload the archive
		m_utilityArchive.Unload();

		// Read to close
		m_isClosed = BtTrue;
	}
	else
	{
		UpdateTest();
	}
}

//////////////////////////////////////////////////////////////////
// SetupRender

void SbMain::SetupRender()
{
	// Apply the shader
	m_pShader->Apply();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( m_camera.GetCamera() );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	//pRenderTarget->SetClearColour( RsColour( 0.5f, 0.6f, 0.7f, 1.0f ) );
	pRenderTarget->SetClearColour( RsColour::NoColour() );

	// Apply this render target
	pRenderTarget->Apply();
}

//////////////////////////////////////////////////////////////////
// SetupRenderToTexture

void SbMain::SetupRenderToTexture( RsTexture *pTexture, RsCamera camera )
{
	// Apply the shader
	m_pShader->Apply();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the texture
	pRenderTarget->SetTexture( pTexture );

	// Set the camera
	pRenderTarget->SetCamera( camera );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour::NoColour() );

	// Apply this render target
	pRenderTarget->Apply();
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
	pRenderTarget->SetClearColour( RsColour( 0.98f, 0.98f, 1.0f ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// Render2DInto3D

void SbMain::Render2DInto3D( RsCamera &camera )
{
	// This code perfectly frames a billboard in front of the camera
	BtFloat completeFieldOfView = camera.GetFieldOfView();
    MtMatrix3 m3Orientation = camera.GetRotation().GetInverse();
	MtVector3 v3AxisXScaled = m3Orientation.XAxis() * 0.5f;
	MtVector3 v3AxisYScaled = m3Orientation.YAxis() * 0.5f;
	MtVector3 v3AxisZ       = m3Orientation.ZAxis();
	
	BtFloat fieldOfView = completeFieldOfView * 0.5f;
	BtFloat tanAngle = MtTan(fieldOfView);
	BtFloat distance = 0.5f / tanAngle;
        
	MtVector3 v3Position = camera.GetPosition() + ( v3AxisZ * distance );
	//

	{
		m_v3Vertex[0].m_v3Position.x = v3Position.x - v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[0].m_v3Position.y = v3Position.y - v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[0].m_v3Position.z = v3Position.z - v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[0].m_v2UV.x = 0;
		m_v3Vertex[0].m_v2UV.y = 0;

		m_v3Vertex[1].m_v3Position.x = v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[1].m_v3Position.y = v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[1].m_v3Position.z = v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[1].m_v2UV.x = 1;
		m_v3Vertex[1].m_v2UV.y = 0;

		m_v3Vertex[2].m_v3Position.x = v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[2].m_v3Position.y = v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[2].m_v3Position.z = v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[2].m_v2UV.x = 0;
		m_v3Vertex[2].m_v2UV.y = 1;

		// Second triangle
		m_v3Vertex[3].m_v3Position.x = v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[3].m_v3Position.y = v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[3].m_v3Position.z = v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[3].m_v2UV.x = 0;
		m_v3Vertex[3].m_v2UV.y = 1;

		m_v3Vertex[4].m_v3Position.x = v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[4].m_v3Position.y = v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[4].m_v3Position.z = v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[4].m_v2UV.x = 1;
		m_v3Vertex[4].m_v2UV.y = 0;

		m_v3Vertex[5].m_v3Position.x = v3Position.x + v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[5].m_v3Position.y = v3Position.y + v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[5].m_v3Position.z = v3Position.z + v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[5].m_v2UV.x = 1;
		m_v3Vertex[5].m_v2UV.y = 1;

		for(BtU32 i = 0; i < 6; i++)
		{
			m_v3Vertex[i].m_colour = RsColour::WhiteColour().asWord();
			m_v3Vertex[i].m_v3Normal = MtVector3(0, 1, 0);
		}
	}
	m_pGUIRenderTarget->Render(RsPT_TriangleList, m_v3Vertex, 6, MaxSortOrders - 1, BtTrue);
}

////////////////////////////////////////////////////////////////////////////////
// Render2DScene

void SbMain::Render2DScene()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render3DScene

void SbMain::Render3DScene()
{
	m_model.Render();
	m_joysticks.Render();

	m_murmuration.Render(&m_camera.GetCamera());
	//m_font.Render();
}

////////////////////////////////////////////////////////////////////////////////
// RenderTests

void SbMain::RenderTests()
{
	m_camera.Render();

	// Apply the shader
	m_pShader->Apply();

	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();

	// Render to the GUI
    RsCamera camera;
    camera.SetProjection(MtMatrix4::m_identity);
    camera.SetDimension(MtVector2(1024.0f, 1024.0f));
    camera.SetViewport(RsViewport(0, 0, 1024, 1024));
    camera.SetAspect(1.0f);
    camera.Update();
    SetupRenderToTexture( m_pGUIRenderTarget->GetTexture(0), camera );
    Render2DScene();

	RestoreRenderTarget();
	//m_skybox.Render();
	Render3DScene();
	Render2DScene();

	MtVector2 v2Dimension = ShHMD::GetDimension();
	v2Dimension.y /= 2.0f;
	//m_pWhite2->Render(MtVector2(0, 0), v2Dimension, RsColour::WhiteColour(), MaxSortOrders - 1);

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

	// Render the mouse
	HlMouse::Render();

	HlDebug::Render();
}

////////////////////////////////////////////////////////////////////////////////
// PreRender

void SbMain::PreRender()
{
}

////////////////////////////////////////////////////////////////////////////////
// PostRender

void SbMain::PostRender()
{
	// Render the mouse
	//m_renderMouse.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbMain::Destroy()
{
}
