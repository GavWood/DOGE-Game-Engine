///////////////////////////////////////////////////////////////////////////////
// RsCamera.h

#pragma once
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtVector3.h"
#include "MtMatrix3.h"
#include "MtMatrix4.h"
#include "RsFrustum.h"
#include "RsViewport.h"

// Class declaration
class RsCamera
{
public:

	RsCamera();
	RsCamera( BtFloat near_plane,
			  BtFloat far_plane,
			  BtFloat aspect_ratio,
			  const RsViewport& viewPort,
			  BtFloat field_of_view );

	RsCamera( BtFloat width,
			  BtFloat height,
			  BtFloat near_plane,
			  BtFloat far_plane,
			  const RsViewport& viewPort );

	void					Update();

	void					LookAt( const MtVector3& v3Position, const MtVector3& v3LookAt, const MtVector3& v3Up );

	// Accessors
	void					SetPosition( const MtVector3& v3Position );
	const MtVector3&		GetPosition() const;

	const MtMatrix4&		GetProjection() const;
	
	void					SetRotation( const MtMatrix3& m3Orientation );
	const MtMatrix3&		GetRotation() const;

	const MtMatrix4&		GetView() const;

	const MtMatrix4&		GetViewProjection() const;
	const MtMatrix4&		GetViewProjectionTexture() const;
	void					SetViewProjection( const MtMatrix4& m4ViewScreen );
	void					SetProjection( const MtMatrix4& m4Projection );

	const RsFrustum&		GetFrustum() const;

	void					SetViewport( const RsViewport& viewport );
	const RsViewport&		GetViewport() const;

	void					FieldOfView( BtFloat fieldOfView );
	BtFloat					FieldOfView() const;

	void					SetWidth( BtFloat width );
	void					SetHeight( BtFloat height );

	BtFloat					GetWidth() const;
	BtFloat					GetHeight() const;

	MtVector2				GetDimension() const;
	void					SetDimension( const MtVector2 &v2Dimension );

	BtFloat					NearPlane() const;
	void					NearPlane( BtFloat nearPlane );
	BtFloat					FarPlane() const;
	void					FarPlane( BtFloat farPlane );

	BtFloat					GetAspect() const;
	void					SetAspect( BtFloat aspect );

	void					SetAngle( BtFloat angle );

	BtBool					GetPerspective() const;
	void					SetPerspective( BtBool isPerspective );

	void					BuildProjectionMatrix();

	BtFloat					GetFieldOfView();

private:

	BtFloat					m_aspect;
	BtFloat					m_nearPlane;
	BtFloat					m_farPlane;
	BtFloat					m_fieldOfView;
	BtBool					m_isPerspective;
	BtFloat					m_width;
	BtFloat					m_height;

	// Member variables
	MtVector3				m_v3Position;
	MtMatrix3				m_m3Orientation;
	MtMatrix4				m_m4Projection;
	MtMatrix4				m_m4View;	
	MtMatrix4				m_m4ViewProjection;
	MtMatrix4				m_m4ViewProjectionTexture;
	RsFrustum				m_frustum;
	RsViewport				m_viewport;
};

///////////////////////////////////////////////////////////////////////////////
// Position

inline void RsCamera::SetPosition( const MtVector3& v3Position )
{
	m_v3Position = v3Position;
}

///////////////////////////////////////////////////////////////////////////////
// GetPosition

inline const MtVector3& RsCamera::GetPosition() const
{
	return m_v3Position;
}

///////////////////////////////////////////////////////////////////////////////
// Projection

inline const MtMatrix4& RsCamera::GetProjection() const
{
	return m_m4Projection;
}

///////////////////////////////////////////////////////////////////////////////
// SetRotation

inline void RsCamera::SetRotation( const MtMatrix3& m3Orientation )
{
	m_m3Orientation = m3Orientation;
}

///////////////////////////////////////////////////////////////////////////////
// GetRotation

inline const MtMatrix3& RsCamera::GetRotation() const
{
	return m_m3Orientation;
}

///////////////////////////////////////////////////////////////////////////////
// GetViewProjection

inline const MtMatrix4& RsCamera::GetViewProjection() const
{
	return m_m4ViewProjection;
}

///////////////////////////////////////////////////////////////////////////////
// GetView

inline const MtMatrix4& RsCamera::GetView() const
{
	return m_m4View;
}

///////////////////////////////////////////////////////////////////////////////
// GetFrustum

inline const RsFrustum& RsCamera::GetFrustum() const
{
	return m_frustum;
}

///////////////////////////////////////////////////////////////////////////////
// Viewport

inline const RsViewport& RsCamera::GetViewport() const
{
	return m_viewport;
}

///////////////////////////////////////////////////////////////////////////////
// FieldOfView

inline void RsCamera::FieldOfView( BtFloat fieldOfView )
{
	m_fieldOfView = fieldOfView;
}

///////////////////////////////////////////////////////////////////////////////
// Viewport

inline BtFloat RsCamera::FieldOfView() const
{
	return m_fieldOfView;
}

///////////////////////////////////////////////////////////////////////////////
// NearPlane

inline BtFloat RsCamera::NearPlane() const
{
	return m_nearPlane;
}

///////////////////////////////////////////////////////////////////////////////
// FarPlane

inline BtFloat RsCamera::FarPlane() const
{
	return m_farPlane;
}

///////////////////////////////////////////////////////////////////////////////
// NearPlane

inline void RsCamera::NearPlane( BtFloat nearPlane )
{
	m_nearPlane = nearPlane;
}

///////////////////////////////////////////////////////////////////////////////
// FarPlane

inline void RsCamera::FarPlane( BtFloat farPlane )
{
	m_farPlane = farPlane;
}

///////////////////////////////////////////////////////////////////////////////
// Aspect

inline BtFloat RsCamera::GetAspect() const
{
	return m_aspect;
}

inline void RsCamera::SetAspect( BtFloat aspect )
{
	m_aspect = aspect;
}

inline void RsCamera::SetWidth( BtFloat width )
{
	m_width = width;
}

inline void RsCamera::SetHeight( BtFloat height )
{
	m_height = height;
}

inline BtFloat RsCamera::GetHeight() const
{
	return m_height;
}

inline BtFloat RsCamera::GetWidth() const
{
	return m_width;
}

inline void RsCamera::SetAngle( BtFloat angle )
{
	m_fieldOfView = angle;
}

inline void RsCamera::SetDimension( const MtVector2 &v2Dimension )
{
	m_width  = v2Dimension.x;
	m_height = v2Dimension.y;
}

inline MtVector2 RsCamera::GetDimension() const
{
	return MtVector2( m_width, m_height );
}

///////////////////////////////////////////////////////////////////////////////
// SetViewport

inline void RsCamera::SetViewport( const RsViewport& viewport )
{
	m_viewport = viewport;
}

///////////////////////////////////////////////////////////////////////////////
// GetViewProjectionTexture

inline const MtMatrix4& RsCamera::GetViewProjectionTexture() const
{
	return m_m4ViewProjectionTexture;
}

///////////////////////////////////////////////////////////////////////////////
// SetPerspective

inline void RsCamera::SetPerspective( BtBool isPerspective )
{
	m_isPerspective = isPerspective;
}

///////////////////////////////////////////////////////////////////////////////
// GetPerspective

inline BtBool RsCamera::GetPerspective() const
{
	return m_isPerspective;
}
