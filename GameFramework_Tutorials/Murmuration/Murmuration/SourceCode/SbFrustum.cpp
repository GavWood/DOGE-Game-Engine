////////////////////////////////////////////////////////////////////////////////
// SbFrustum.cpp

// Includes
#include "SbFrustum.h"
#include "RsRenderTarget.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbFrustum::Setup( BaArchive *pArchive )
{
	m_pWhite3 = pArchive->GetMaterial( "white3" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbFrustum::Update( const MtMatrix3 &m3Rotation, MtVector3 v3Position, BtFloat completeFieldOfView, BtFloat aspect )
{
	BtFloat fieldOfView = completeFieldOfView * 0.5f;

	BtFloat z = 20.0f;
	BtFloat tanAngle = MtTan( fieldOfView );
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

void SbFrustum::Render()
{
	return;
	/*
	RsColour black = RsColour::BlackColour();
	RsColour green = RsColour::GreenColour();

	RenderLine( m_rect[0], m_rect[1], green, MaxSortOrders - 1 );
	RenderLine( m_rect[0], m_rect[2], green, MaxSortOrders - 1 );
	RenderLine( m_rect[0], m_rect[3], green, MaxSortOrders - 1 );
	RenderLine( m_rect[0], m_rect[4], green, MaxSortOrders - 1 );
	RenderLine( m_rect[1], m_rect[3], black, MaxSortOrders - 1 );
	RenderLine( m_rect[3], m_rect[4], black, MaxSortOrders - 1 );
	RenderLine( m_rect[1], m_rect[2], black, MaxSortOrders - 1 );
	RenderLine( m_rect[4], m_rect[2], black, MaxSortOrders - 1 );
	*/
}

////////////////////////////////////////////////////////////////////////////////
// GetVertex

MtVector3 *SbFrustum::GetVertex()
{
	return m_rect;
}