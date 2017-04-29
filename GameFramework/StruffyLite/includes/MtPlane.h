////////////////////////////////////////////////////////////////////////////////
// MtPlane.h

// Include guard
#pragma once
#include "MtVector3.h"

class MtTriangle;

// Class declaratiion
class MtPlane
{
public:

	// Constructors

	MtPlane() {};
	MtPlane( const MtVector3 &v3Normal, BtFloat fDistance );
	MtPlane( const MtTriangle* );
	MtPlane( const MtVector3 &v3Normal, const MtVector3 &v3Point );
	MtPlane( const MtVector3 &v3A, const MtVector3 &v3B, const MtVector3 &v3C );

	// Calculate the which side of a plane a point is on
	BtFloat								SideOfPlane( const MtVector3 &v3Point ) const;

	// Calculate the shortest distance from a point to a plane
	const BtFloat						DistanceTo( const MtVector3& v3Point ) const;

	// Calculate the closest position on a plane from a point
	MtVector3							ClosestPointOnPlane( const MtVector3& v3Point ) const;

	// Give a position and a direction calculate whether this vector will intersect the plane
	BtBool								Intersect( const MtVector3 &v3Position, const MtVector3 &v3Direction, MtVector3 &v3Intersection );

	// Member variables
	MtVector3							m_v3Normal;

	BtFloat								m_fDistance;
};
