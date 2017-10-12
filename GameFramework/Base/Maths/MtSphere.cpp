////////////////////////////////////////////////////////////////////////////////
// MtSphere.cpp

#include "MtVector3.h"
#include "MtSphere.h"
#include "MtMath.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

MtSphere::MtSphere()
{
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

MtSphere::MtSphere( const MtVector3& v3Position, BtFloat radius )
{
	m_v3Position = v3Position;
	m_radius = radius;
}

////////////////////////////////////////////////////////////////////////////////
// SphericalTest

BtBool MtSphere::SphericalTest( const MtSphere& other )
{
	BtFloat t = (m_radius + other.m_radius);
	BtFloat temp_dist = t * t;

	if( ( ( ( other.m_v3Position.x - m_v3Position.x ) * ( other.m_v3Position.x - m_v3Position.x ) )+
	 	  ( ( other.m_v3Position.y - m_v3Position.y ) * ( other.m_v3Position.y - m_v3Position.y ) )+
 		  ( ( other.m_v3Position.z - m_v3Position.z ) * ( other.m_v3Position.z - m_v3Position.z ) )
     ) < temp_dist )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// ExpandBy

void MtSphere::ExpandBy( const MtVector3& v3Position )
{
	MtVector3 v3DistanceFromOrigin = v3Position - m_v3Position;

	BtFloat distanceFromOrigin = v3DistanceFromOrigin.GetLength();

	m_radius = MtMax( distanceFromOrigin, m_radius );
}

////////////////////////////////////////////////////////////////////////////////
// ExpandBy

void MtSphere::ExpandBy( const MtSphere& other )
{
	MtVector3 v3DirectionToSphere = other.m_v3Position - m_v3Position;

	if( ( v3DirectionToSphere.x == 0 ) && ( v3DirectionToSphere.y == 0 ) && ( v3DirectionToSphere.z == 0 ) )
	{
		return;
	}

	MtVector3 v3Normalised = v3DirectionToSphere.GetNormalise();

	MtVector3 v3ToPerimeter = v3Normalised * other.m_radius;

	MtVector3 v3Perimeter = other.m_v3Position + v3ToPerimeter;
	
	ExpandBy( v3Perimeter );
}

////////////////////////////////////////////////////////////////////////////////
// Transform sphere

void MtSphere::Transform( const MtMatrix4& m4Transform )
{
	m_v3Position = m_v3Position * m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// Intersect

// http://www.devmaster.net/wiki/Talk:Ray-sphere_intersection

BtBool MtSphere::Intersect( MtVector3 rayStart,
						    MtVector3 rayEnd,
							MtVector3& v3Intersection1,
							MtVector3& v3Intersection2
							) const
{
	BtFloat radius2 = m_radius * m_radius;

	MtVector3 v3Direction = rayEnd - rayStart;

	v3Direction.Normalise();

	MtVector3 dst = rayStart - m_v3Position;

	BtFloat B = dst.DotProduct( v3Direction);			
	BtFloat C = dst.DotProduct( dst)  - radius2;
	BtFloat D = B * B - C;							
	
	if( D < 0 )
	{
		return BtFalse;
	}

	BtFloat sqrtD = MtSqrt(D);		
																						
	BtFloat t0 = (-B - sqrtD);					
	BtFloat t1 = (-B + sqrtD);				
	
	v3Intersection1 = rayStart + ( v3Direction * t0 );
	v3Intersection2 = rayStart + ( v3Direction * t1 );
	return BtTrue;
}
