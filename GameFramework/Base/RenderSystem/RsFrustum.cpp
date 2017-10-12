////////////////////////////////////////////////////////////////////////////////
// RsFrustum.cpp

#include "RsFrustum.h"
#include "MtMath.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

RsFrustum::RsFrustum()
{
	m_frustumPointMax = MtVector3(0.0f, 0.0f, 0.0f);
	m_frustumPointMin = MtVector3(0.0f, 0.0f, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
// Set

void RsFrustum::Set( const MtMatrix4& m4Transform )
{
	// Right Plane
	m_frustumPlanes[0].m_v3Normal.x = m4Transform[0][3] - m4Transform[0][0];
	m_frustumPlanes[0].m_v3Normal.y = m4Transform[1][3] - m4Transform[1][0];
	m_frustumPlanes[0].m_v3Normal.z = m4Transform[2][3] - m4Transform[2][0];
	m_frustumPlanes[0].m_fDistance  = m4Transform[3][3] - m4Transform[3][0];

	// Left Plane
	m_frustumPlanes[1].m_v3Normal.x = m4Transform[0][3] + m4Transform[0][0];
	m_frustumPlanes[1].m_v3Normal.y = m4Transform[1][3] + m4Transform[1][0];
	m_frustumPlanes[1].m_v3Normal.z = m4Transform[2][3] + m4Transform[2][0];
	m_frustumPlanes[1].m_fDistance  = m4Transform[3][3] + m4Transform[3][0];

	// Bottom Plane
	m_frustumPlanes[2].m_v3Normal.x = m4Transform[0][3] + m4Transform[0][1];
	m_frustumPlanes[2].m_v3Normal.y = m4Transform[1][3] + m4Transform[1][1];
	m_frustumPlanes[2].m_v3Normal.z = m4Transform[2][3] + m4Transform[2][1];
	m_frustumPlanes[2].m_fDistance  = m4Transform[3][3] + m4Transform[3][1];

	// Top Plane
	m_frustumPlanes[3].m_v3Normal.x = m4Transform[0][3] - m4Transform[0][1];
	m_frustumPlanes[3].m_v3Normal.y = m4Transform[1][3] - m4Transform[1][1];
	m_frustumPlanes[3].m_v3Normal.z = m4Transform[2][3] - m4Transform[2][1];
	m_frustumPlanes[3].m_fDistance  = m4Transform[3][3] - m4Transform[3][1];

	// Far Plane
	m_frustumPlanes[4].m_v3Normal.x = m4Transform[0][3] - m4Transform[0][2];
	m_frustumPlanes[4].m_v3Normal.y = m4Transform[1][3] - m4Transform[1][2];
	m_frustumPlanes[4].m_v3Normal.z = m4Transform[2][3] - m4Transform[2][2];
	m_frustumPlanes[4].m_fDistance  = m4Transform[3][3] - m4Transform[3][2];

	// Near Plane
	m_frustumPlanes[5].m_v3Normal.x = m4Transform[0][3];
	m_frustumPlanes[5].m_v3Normal.y = m4Transform[1][3];
	m_frustumPlanes[5].m_v3Normal.z = m4Transform[2][3];
	m_frustumPlanes[5].m_fDistance  = m4Transform[3][3];

	for( BtU32 i=0; i<6; i++ )
	{
		MtPlane& plane = m_frustumPlanes[i];
		BtFloat Scale = 1.0f / plane.m_v3Normal.GetLength();
		m_frustumPlanes[i] = MtPlane( plane.m_v3Normal * -Scale, plane.m_fDistance * -Scale );
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsInside

BtBool RsFrustum::IsInside( const MtVector3& v3Point ) const
{
	for( BtU32 iPlane = 0; iPlane < 6; ++iPlane )
	{
		if( m_frustumPlanes[iPlane].DistanceTo( v3Point ) > 0 )
		{
			return BtFalse;
		}
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// IsInside

BtBool RsFrustum::IsInside( const MtVector3& v3Point, BtFloat radius ) const
{
	for( BtU32 iPlane = 0; iPlane < 6; ++iPlane )
	{
		BtFloat fDistanceToPlane = m_frustumPlanes[iPlane].DistanceTo( v3Point );

		if( fDistanceToPlane > radius )
		{
			return BtFalse;
		}
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// GetNearestPointOnFrustum

MtVector3 RsFrustum::GetNearestPointOnFrustum( const MtVector3& v3Position ) const
{
	BtFloat fNearestDistance = m_frustumPlanes[0].DistanceTo( v3Position );
	MtVector3 v3NearestPoint = MtVector3( v3Position - m_frustumPlanes[0].m_v3Normal * fNearestDistance );

	for( BtU32 iPlane = 1; iPlane < 6; ++iPlane )
	{
		BtFloat fDistance = m_frustumPlanes[iPlane].DistanceTo( v3Position );

		if( MtAbs( fDistance ) < MtAbs( fNearestDistance ) )
		{
			v3NearestPoint = MtVector3( v3Position - m_frustumPlanes[iPlane].m_v3Normal * fDistance );
			fNearestDistance = fDistance;
		}
	}
	return v3NearestPoint;
}


