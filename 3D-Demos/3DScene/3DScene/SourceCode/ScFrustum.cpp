////////////////////////////////////////////////////////////////////////////////
// SbFrustum.cpp

// Includes
#include "ScFrustum.h"
#include "RsRenderTarget.h"
#include "ApConfig.h"
#include "HlDraw.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void ScFrustum::Setup( BaArchive *pArchive )
{
	m_pWhite3 = pArchive->GetMaterial( "white3" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ScFrustum::Update( const MtMatrix3 &m3Rotation, const MtVector3& v3Position, BtFloat completeFieldOfViewInRadians, BtFloat aspect )
{
	BtFloat halfFieldOfView = completeFieldOfViewInRadians * 0.5f;

	BtFloat z = 20.0f;
	BtFloat tanAngle = MtTan( halfFieldOfView );
	BtFloat x = z * tanAngle;
	BtFloat y = z * tanAngle;
	x = x * aspect;

	m_rect[0] = MtVector3( 0, 0, 0 );
	m_rect[1] = MtVector3(-x,-y, z );
	m_rect[2] = MtVector3(-x, y, z );
	m_rect[3] = MtVector3( x,-y, z );
	m_rect[4] = MtVector3( x, y, z );

	for( BtU32 i=0; i<5; i++ )
	{
		m_rect[i] *= m3Rotation.GetInverse();
		m_rect[i] += v3Position;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void ScFrustum::Render()
{
	if (ApConfig::IsDebug())
	{
		RsColour black = RsColour::BlackColour();
		RsColour green = RsColour::GreenColour();

		HlDraw::RenderLine( m_rect[0], m_rect[1], green, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[0], m_rect[2], green, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[0], m_rect[3], green, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[0], m_rect[4], green, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[1], m_rect[3], black, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[3], m_rect[4], black, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[1], m_rect[2], black, MaxSortOrders - 1);
		HlDraw::RenderLine( m_rect[4], m_rect[2], black, MaxSortOrders - 1);
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetVertex

MtVector3 *ScFrustum::GetVertex()
{
	return m_rect;
}