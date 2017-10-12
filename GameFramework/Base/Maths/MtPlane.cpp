// Include
#include "MtVector3.h"
#include "MtVector4.h"
#include "MtPlane.h"
#include "MtTriangle.h"

// Constructors
MtPlane::MtPlane(const MtTriangle* pTriangle)
{
   m_v3Normal  = pTriangle->normal;
   m_fDistance = -pTriangle->a.DotProduct( m_v3Normal );
}

MtPlane::MtPlane(const MtVector3 &normal, BtFloat fDistance )
{
   m_v3Normal  = normal;
   m_fDistance = fDistance;
}

MtPlane::MtPlane(const MtVector3 &normal, const MtVector3 &v3Point)
{
	m_v3Normal  = normal;
	m_fDistance = -v3Point.DotProduct( m_v3Normal );
}

MtPlane::MtPlane( const MtVector3 &v3A, const MtVector3 &v3B, const MtVector3 &v3C )
{
	MtTriangle triangle = MtTriangle( v3A, v3B, v3C );
	triangle.FindNormal();
	*this = MtPlane( &triangle );
}

// Public functions
BtFloat MtPlane::SideOfPlane( const MtVector3 &v3Point ) const
{
	BtFloat fDistance = DistanceTo( v3Point );
   
	return fDistance;
}

const BtFloat MtPlane::DistanceTo( const MtVector3& v3Point ) const
{
	return m_v3Normal.DotProduct( v3Point ) + m_fDistance;
}

MtVector3 MtPlane::ClosestPointOnPlane( const MtVector3& v3Point ) const
{
	BtFloat fDistance = DistanceTo( v3Point );

	return MtVector3( v3Point - m_v3Normal * fDistance );
}

BtBool MtPlane::Intersect( const MtVector3 &v3Position, const MtVector3 &v3Direction, MtVector3 &v3Intersection )
{
	BtFloat fDot = -v3Direction.DotProduct( m_v3Normal );

	BtFloat fDistance = (m_v3Normal.DotProduct( v3Position ) + m_fDistance ) / fDot;

	v3Intersection = v3Position + ( v3Direction * fDistance );

	return BtTrue;
}

	