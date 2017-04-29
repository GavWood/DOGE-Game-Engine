// Local includes /////////////////////////////////////////////////////////////
#include "MtVector3.h"
#include "MtAABB.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

MtAABB::MtAABB()
{
	m_min = MtVector3( 0, 0, 0 );
	m_max = MtVector3( 0, 0, 0 );
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

MtAABB::MtAABB( const MtVector3& Min, const MtVector3& Max )
{
	m_min = Min;
	m_max = Max;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

MtAABB::MtAABB( const MtVector3& v3Position )
{
	m_min = v3Position;
	m_max = v3Position;
}

MtAABB::MtAABB( BtFloat x1, BtFloat y1, BtFloat z1, BtFloat x2, BtFloat y2, BtFloat z2 )
{
	m_min = MtVector3( x1, y1, z1 );
	m_max = MtVector3( x2, y2, z2 );
}

////////////////////////////////////////////////////////////////////////////////
// Center

MtVector3 MtAABB::Center() const
{
	return (m_min + m_max) * 0.5f;
};

////////////////////////////////////////////////////////////////////////////////
// Radius

 BtFloat MtAABB::Radius() const
{
	// Calculate one corner of the box to the other
	MtVector3 v3Delta = m_max - m_min;

	// Get the length of the vector. This is the diameter of the bounding sphere.
	BtFloat fMagnitude = v3Delta.GetLength();

	// Return the magnitude
	return fMagnitude * 0.5f;
}

 ////////////////////////////////////////////////////////////////////////////////
 // Diameter

 BtFloat MtAABB::Diameter() const
 {
	 // Calculate one corner of the box to the other
	 MtVector3 v3Delta = m_max - m_min;

	 // Get the length of the vector. This is the diameter of the bounding sphere.
	 BtFloat fMagnitude = v3Delta.GetLength();

	 // Return the magnitude
	 return fMagnitude;
 }

////////////////////////////////////////////////////////////////////////////////
// Intersect

 BtBool MtAABB::Intersect(MtAABB const& AABB ) const
{
	return( m_min.x < ( AABB.Max().x ) &&
			m_min.y < ( AABB.Max().y ) &&
			m_min.z < ( AABB.Max().z ) &&
			m_max.x > ( AABB.Min().x ) &&
			m_max.y > ( AABB.Min().y ) &&
			m_max.z > ( AABB.Min().z ) );
}

////////////////////////////////////////////////////////////////////////////////
// Contains

 BtBool MtAABB::Contains(MtAABB const& AABB ) const
{
	return( m_min.x < AABB.Min().x &&
			m_max.x > AABB.Max().x &&

			m_min.y < AABB.Min().y &&
			m_max.y > AABB.Max().y &&

			m_min.z < AABB.Min().z &&
			m_max.z > AABB.Max().z );
}

////////////////////////////////////////////////////////////////////////////////
// TestPoint

 BtBool MtAABB::TestPoint(const MtVector3& point ) const
{
	if( point.x < m_min.x || point.y < m_min.y || point.z < m_min.z ||
		point.x > m_max.x || point.y > m_max.y || point.z > m_max.z )
	{
		return BtFalse;
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Transform

MtAABB MtAABB::Transform( const MtMatrix4& Transform )
{
	MtAABB aabb;
	aabb.m_min = m_min * Transform;
	aabb.m_max = m_max * Transform;
	return aabb;
}

////////////////////////////////////////////////////////////////////////////////
// Dimension

 MtVector3 MtAABB::Dimension() const
{
	return m_max - m_min;
}

////////////////////////////////////////////////////////////////////////////////
// ExpandBy

 void MtAABB::ExpandBy( const MtAABB& aabb )
{
	ExpandBy( aabb.m_min );
	ExpandBy( aabb.m_max );
}

////////////////////////////////////////////////////////////////////////////////
// ExpandBy

 void MtAABB::ExpandBy( const MtVector3& v3Position )
{
	m_min.X( MtMin( m_min.X(), v3Position.X() ) );
	m_min.Y( MtMin( m_min.Y(), v3Position.Y() ) );
	m_min.Z( MtMin( m_min.Z(), v3Position.Z() ) );

	m_max.X( MtMax( m_max.X(), v3Position.X() ) );
	m_max.Y( MtMax( m_max.Y(), v3Position.Y() ) );
	m_max.Z( MtMax( m_max.Z(), v3Position.Z() ) );
}
