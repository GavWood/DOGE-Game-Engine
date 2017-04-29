///////////////////////////////////////////////////////////////////////////////
// RsCamera.cpp

// Local includes
#include "RsCamera.h"
#include "MtMath.h"
#include "ApConfig.h"
#include "BtBase.h"
#include "RsUtil.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor

RsCamera::RsCamera()
{
	m_nearPlane   = 0.1f;
	m_farPlane    = 10000.0f;
	m_fieldOfView = MtDegreesToRadians( 60.0f );

	m_v3Position = MtVector3(0, 0, 0);	
	m_m3Orientation.SetIdentity();

	m_viewport = RsViewport( 0, 0, 0, 0 );
	m_aspect = 0;
	m_isPerspective = BtTrue;

	m_width = 0;
	m_height = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Constructor

RsCamera::RsCamera( BtFloat near_plane,
				    BtFloat far_plane,
				    BtFloat aspect_ratio,
					const RsViewport& viewPort,
					BtFloat field_of_view )
{
	m_nearPlane   = near_plane;
	m_farPlane    = far_plane;
	m_fieldOfView = field_of_view;
    m_width       = (BtFloat)RsUtil::GetWidth();
    m_height      = (BtFloat)RsUtil::GetHeight();

	m_v3Position = MtVector3(0, 0, 0);	
	m_m3Orientation.SetIdentity();

	m_aspect = aspect_ratio;
	m_viewport = viewPort;
	m_isPerspective = BtTrue;
	BuildProjectionMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// Constructor

RsCamera::RsCamera( BtFloat width,
					BtFloat height,
					BtFloat near_plane,
					BtFloat far_plane,
					const RsViewport& viewPort )
{
	m_isPerspective = BtFalse;
	m_width = width;
	m_height = height;
	m_nearPlane = near_plane;
	m_farPlane = far_plane;
	m_viewport = viewPort;
	m_aspect = width / height;

	BuildProjectionMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// GetFieldOfView

BtFloat	RsCamera::GetFieldOfView()
{
	return m_fieldOfView; 
}

// http://www.geeks3d.com/20090729/howto-perspective-projection-matrix-in-opengl/

///////////////////////////////////////////////////////////////////////////////
// BuildProjectionMatrix

void RsCamera::BuildProjectionMatrix()
{
	// BuildProjectionMatrix failed as width was 0
	BtAssert( m_width != 0 );

	// BuildProjectionMatrix failed as height was 0
	BtAssert( m_height != 0 );

	// Build the projection matrix
	if( m_isPerspective == BtTrue )
	{
		// Camera aspect ratio must not be 0
		BtAssert( m_aspect != 0 );

		m_m4Projection.BuildLeftHandedProjectionMatrix( m_nearPlane, m_farPlane, m_aspect, m_fieldOfView );
	}
	else
	{
		m_m4Projection.BuildOrthographicProjectionMatrix( m_width, m_height, m_nearPlane, m_farPlane );
	}
}

///////////////////////////////////////////////////////////////////////////////
// Update

void RsCamera::Update()
{
	// Setup the translation part
	MtMatrix4 m4Translation;
	m4Translation.SetTranslation( -m_v3Position );

	// Calculate the view transform
	m_m4View = m4Translation * m_m3Orientation;

	// Calculate the world to screen transform
	m_m4ViewProjection = m_m4View * m_m4Projection;

	MtMatrix4 matTexAdj;

	if( ApConfig::GetPlatform() == ApPlatform_DX11 )
	{
		matTexAdj = MtMatrix4( 0.5f,      0.0f,     0.0f, 0,
							   0.0f,     -0.5f,     0.0f, 0,
							   0.0f,      0.0f,     0.5f, 0,
							   0.5f,      0.5f,	    0.5f, 1.0f );
		m_m4ViewProjectionTexture = m_m4ViewProjection * matTexAdj;
	}
	else
	{
		matTexAdj = MtMatrix4( 0.5f, 0.0f, 0.0f, 0,
							   0.0f, 0.5f, 0.0f, 0,
							   0.0f, 0.0f, 0.5f, 0,
							   0.5f, 0.5f, 0.5f, 1.0f);

		m_m4ViewProjectionTexture = m_m4ViewProjection * matTexAdj;
	}
    
	// Update the frustum
	m_frustum.Set( m_m4ViewProjection );

	BuildProjectionMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// SetProjection

void RsCamera::SetProjection( const MtMatrix4& m4Projection )
{
	m_m4Projection = m4Projection;
}

///////////////////////////////////////////////////////////////////////////////
// SetViewProjection

void RsCamera::SetViewProjection( const MtMatrix4& m4ViewScreen )
{
	m_m4ViewProjection = m4ViewScreen;
	m_frustum.Set( m_m4ViewProjection );
}

//////////////////////////////////////////////////////////////////////////
// LookAt 

void RsCamera::LookAt( const MtVector3& v3Position, const MtVector3& v3LookAt, const MtVector3& v3Up )
{
	MtVector3 v3Z = v3LookAt - v3Position;
	v3Z.Normalize();

	MtVector3 v3X = MtVector3::CrossProduct( v3Up, v3Z );
	v3X.Normalize();

	MtVector3 v3Y = MtVector3::CrossProduct( v3Z, v3X );
	v3Y.Normalize();

	MtMatrix4 m4Orientation;
	m4Orientation.Row0( MtVector4( v3X.X(),  v3Y.X(),  v3Z.X(),  0.0f ) );
	m4Orientation.Row1( MtVector4( v3X.Y(),  v3Y.Y(),  v3Z.Y(),  0.0f ) );
	m4Orientation.Row2( MtVector4( v3X.Z(),  v3Y.Z(),  v3Z.Z(),  0.0f ) );
	m4Orientation.Row3( MtVector4(    0.0f,     0.0f,     0.0f,  1.0f ) );

	m_m3Orientation = m4Orientation;
}

