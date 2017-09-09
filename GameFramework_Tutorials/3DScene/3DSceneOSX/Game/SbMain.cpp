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
#include "HlLines.h"
#include "HlMouse.h"
#include "UiKeyboard.h"
#include "RsMaterial.h"
#include "UiKeyboard.h"

//const BtBool useRenderTarget = BtFalse;
//const BtBool useRenderTarget = BtTrue;
double SbMain::m_counter = 0;

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	MtVector2 v2Dimension;

	ApConfig::SetTitle( "Tutorial 1" );

	if( ApConfig::IsWin() )
	{
		v2Dimension = MtVector2(1024.0f, 768.0f );

		RsUtil::SetDimension(v2Dimension);
	}
    
    Resize();

	ApConfig::SetDebug( BtTrue );

	RdRandom::SetRandomSeed();

	// Load the game archive
    ApConfig::SetResourcePath( "\\dev\\engine\\struffy\\tutorial1\\release\\" );
    ApConfig::SetExtension( ".osx" );

    m_counter = 0;
    
	int test = 1;
	int one  = 1;
	if( one && test )
	{
		int a=0;
		a++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Resize

void SbMain::Resize()
{
    MtVector2 v2Dimension = RsUtil::GetDimension();
    m_camera.SetDimension( MtVector2( v2Dimension.x, v2Dimension.y ) );
    m_camera.Init();
    m_camera.SetPosition( MtVector3( 0, 2.0f, -5.0f ) );
    m_camera.SetSpeed( 10.0f );
}

////////////////////////////////////////////////////////////////////////////////
// Create

void SbMain::Create()
{
	m_gameArchive.LoadDictionary();
	m_gameArchive.Load( "game" );

	m_animArchive.LoadDictionary();
	m_animArchive.Load( "anim" );
	m_utilityArchive.LoadDictionary();
	m_utilityArchive.Load("utility");
    
	m_pWhite2 = m_gameArchive.GetMaterial( "white2" );
	m_pWhite3 = m_gameArchive.GetMaterial( "white3" );
	m_pShader = m_gameArchive.GetShader( "shader" );
	 
	m_wolf2.Setup( &m_gameArchive );
	m_skybox.Setup( &m_gameArchive );
	m_model.Setup( &m_gameArchive, &m_animArchive );
	m_texture.Setup( &m_gameArchive );
	
	RsMaterial *pMaterial2 = m_gameArchive.GetMaterial("white2");
	RsMaterial *pMaterial3 = m_gameArchive.GetMaterial("white3");
	RsMaterial *pMaterial3notest = m_gameArchive.GetMaterial("white3noztest");

	HlFont::Setup( &m_utilityArchive, "vera" );
	HlMouse::Setup( &m_utilityArchive );
	HlLines::Setup( pMaterial2, pMaterial3, pMaterial3notest );

	m_pSound = m_gameArchive.GetSound( "howl" );
    
    m_skippingRope.Init();
    
    m_isSkippingRope = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbMain::Reset()
{
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateTest()
{
	m_camera.Update();
	m_skybox.Update( m_camera.GetCamera().GetPosition() );
	m_model.Update();

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_F1))
	{
		ApConfig::SetDebug(!ApConfig::IsDebug());
	}

	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_Y ) )
	{
		m_pSound->Play();
	}
    
    // Update the skipping rope
    if( m_isSkippingRope )
    {
        m_skippingRope.Update();
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
		m_animArchive.Unload();

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
	pRenderTarget->SetClearColour( RsColour( 0.5f, 0.6f, 0.7f, 1.0f ) );
	//pRenderTarget->SetClearColour( RsColour::WhiteColour() );

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
	pRenderTarget->SetClearColour( RsColour( 0, 0, 0, 1.0f ) );

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
	pRenderTarget->SetClearColour( RsColour( 0, 0, 0, 1.0f ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// DrawRenderTarget

void SbMain::DrawRenderTarget()
{
	/*
	if( ApConfig::GetPlatform() != ApPlatform_DX11 )
	{
		HlMaterial::RenderUpsideDown( m_pRenderTarget, MtVector2( 0, 0 ), MtVector2( 256.0f, 256.0f ) );
	}
	else
	{
		HlMaterial::RenderRightWayUp( m_pRenderTarget, MtVector2( 0, 0 ), MtVector2( 256.0f, 256.0f ) );
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////
// RenderUpsideDown

void SbMain::RenderUpsideDown(RsMaterial *pMaterial, const MtVector2 &v2Position, const MtVector2 &v2Dimension, const MtVector2 &v2ScreenDimension )
{
	// Allocate vertex
	RsVertex3 vertex[4];
	RsVertex3 *pVertex = &vertex[0];

	// Calculate the positions
	BtFloat xmax = v2Dimension.x;
	BtFloat ymax = v2Dimension.y;

	RsVertex3 *pQuad = pVertex;

	// Copy these into vertex
	pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y, 0.1f);
	pVertex->m_v2UV = MtVector2(0, 0);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y + ymax, 0.1f);
	pVertex->m_v2UV = MtVector2(0, 1);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + 0, 0.1f);
	pVertex->m_v2UV = MtVector2(1, 0);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + ymax, 0.1f);
	pVertex->m_v2UV = MtVector2(1, 1);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	// Flip the V's
	/*
	pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
	pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
	pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
	pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;
	*/

	// Cache the display width and height
	BtFloat fScaleWidth = 1.0f / v2ScreenDimension.x;
	BtFloat fScaleHeight = 1.0f / v2ScreenDimension.y;

	// Scale the position to local screen space -1 to 1
	for (BtU32 i = 0; i < 4; i++)
	{
		// Flip the y
		pQuad[i].m_v3Position.y = v2ScreenDimension.y - pQuad[i].m_v3Position.y;

		// Scale from 0..width to 0..1
		pQuad[i].m_v3Position.x *= fScaleWidth;
		pQuad[i].m_v3Position.y *= fScaleHeight;

		// Scale from 0..1 to 0..2
		pQuad[i].m_v3Position.x *= 2.0f;
		pQuad[i].m_v3Position.y *= 2.0f;

		// Translate from 0..2 to -1..1
		pQuad[i].m_v3Position.x -= 1.0f;
		pQuad[i].m_v3Position.y -= 1.0f;
	}

	pMaterial->Render(RsPT_TriangleStrip, pQuad, 4, MaxSortOrders - 1);
}

////////////////////////////////////////////////////////////////////////////////
// RenderScene

void SbMain::RenderScene()
{
	m_skybox.Render();
	m_model.Render();
}

////////////////////////////////////////////////////////////////////////////////
// RenderTests

void SbMain::RenderTests()
{
	// Apply the shader
	m_pShader->Apply();

	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();

	RestoreRenderTarget();
	RenderScene();
    
	BtChar text[32];
	sprintf( text, "%.2f %.2f", RsUtil::GetFPS(), m_counter );
	HlFont::Render( MtVector2( 0, 0 ), text, RsColour::WhiteColour(), MaxSortOrders-1 );
    
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

    // Render the skipping rope
    if( m_isSkippingRope )
    {
        m_skippingRope.Render();
    }
    
	// Render the mouse
	HlMouse::Render();
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
