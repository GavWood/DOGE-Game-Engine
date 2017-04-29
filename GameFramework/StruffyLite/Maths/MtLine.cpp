// Includes
#include <math.h>
#include "MtVector3.h"
#include "MtVector2.h"
#include "MtVector4.h"
#include "MtLine.h"
#include "MtTriangle.h"
#include "MtPlane.h"

// Public functions
void MtLine::FindLineRadius()
{
	MtVector3 diff = point1 - point2;

	m_fRadius = (BtFloat) sqrt( (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z) );
}

BtBool MtLine::Collide(const MtTriangle &triangle, MtVector3 &intersection)
{
   // Calculate the plane of the triangle
   MtPlane plane( &triangle );

   // Calculate the intersection
   BtFloat a = plane.m_v3Normal.x;
   BtFloat b = plane.m_v3Normal.y;
   BtFloat c = plane.m_v3Normal.z;
   BtFloat d = plane.m_fDistance;

   BtFloat i = point2.x - point1.x;
   BtFloat j = point2.y - point1.y;
   BtFloat k = point2.z - point1.z;

   BtFloat n_t = -(a * point1.x + b * point1.y + c * point1.z + d);
   BtFloat d_t = (a * i + b * j + c * k);

   if( !d_t && n_t )
      return BtFalse;

   if( !d_t && !n_t )
      return BtFalse;

   BtFloat t = n_t / d_t;

   // Check the intersection happens in the line's segment
   if( t > 1.0f )
      return BtFalse;

   if( t < 0.0f )
      return BtFalse;

   intersection = MtVector3 ( point1.x + i * t, point1.y + j * t, point1.z + k * t );

   // is this point inside the triangle
   MtVector3 v0 = triangle.a;
   MtVector3 v1 = triangle.b;
   MtVector3 v2 = triangle.c;

   MtVector3 ParPolyv0 = v0 + plane.m_v3Normal;
   MtVector3 ParPolyv1 = v1 + plane.m_v3Normal;

   MtTriangle edge_poly1 = MtTriangle( v0, ParPolyv0, v1 );
   edge_poly1.FindNormal();
   MtPlane plane1 = MtPlane( &edge_poly1 );

   if( plane1.DistanceTo( intersection ) < 0 )
       return BtFalse;

   MtTriangle edge_poly2 = MtTriangle( v0, v2, ParPolyv0 );
   edge_poly2.FindNormal();
   MtPlane plane2 = MtPlane( &edge_poly2 );

   if( plane2.DistanceTo( intersection ) < 0 )
       return BtFalse;

   MtTriangle edge_poly3 = MtTriangle( v1, ParPolyv1, v2 );
   edge_poly3.FindNormal();
   MtPlane plane3 = MtPlane( &edge_poly3 );

   if( plane3.DistanceTo( intersection ) < 0 )
       return BtFalse;

   return BtTrue;
}

BtBool MtLine::CollideWithPlane(const MtPlane &plane, MtVector3 &intersection)
{
   // Calculate the intersection
   BtFloat a = plane.m_v3Normal.x;
   BtFloat b = plane.m_v3Normal.y;
   BtFloat c = plane.m_v3Normal.z;
   BtFloat d = plane.m_fDistance;

   BtFloat i = point2.x - point1.x;
   BtFloat j = point2.y - point1.y;
   BtFloat k = point2.z - point1.z;

   BtFloat n_t = -(a * point1.x + b * point1.y + c * point1.z + d);
   BtFloat d_t = (a * i + b * j + c * k);

   if( !d_t && n_t )
      return BtFalse;

   if( !d_t && !n_t )
      return BtFalse;

   BtFloat t = n_t / d_t;

   // Check the intersection happens in the line's segment
   if( t > 1.0f )
      return BtFalse;

   if( t < 0.0f )
      return BtFalse;

   intersection = MtVector3 ( point1.x + i * t, point1.y + j * t, point1.z + k * t );

   return BtTrue;
}

int CheckTriClockDir( MtVector2 pt1, MtVector2 pt2, MtVector2 pt3)
{
	const BtU32 CLOCKWISE = -1;
	const BtU32 COUNTER_CLOCKWISE = 1;
	const BtU32 LINE = 0;

	float test;
	test = (((pt2.x - pt1.x)*(pt3.y - pt1.y)) - ((pt3.x - pt1.x)*(pt2.y - pt1.y))); 
	if (test > 0) return COUNTER_CLOCKWISE;
	else if(test < 0) return CLOCKWISE;
	else return LINE;
}

BtBool MtLine::GetLineIntersection(MtVector2 v2PointA, MtVector2 v2PointB, MtVector2 v2PointC, MtVector2 v2PointD, MtVector2 &v2Output )
{
	if( !IsIntersecting(v2PointA, v2PointB, v2PointC, v2PointD) )
	{
		return BtFalse;
	}

	BtFloat p0_x = v2PointA.x;
	BtFloat p0_y = v2PointA.y;
	BtFloat p1_x = v2PointB.x;
	BtFloat p1_y = v2PointB.y;
	BtFloat p2_x = v2PointC.x;
	BtFloat p2_y = v2PointC.y;
	BtFloat p3_x = v2PointD.x;
	BtFloat p3_y = v2PointD.y;

	BtFloat s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, t_numer, denom, t;
	s10_x = p1_x - p0_x;
	s10_y = p1_y - p0_y;
	s02_x = p0_x - p2_x;
	s02_y = p0_y - p2_y;
	s32_x = p3_x - p2_x;
	s32_y = p3_y - p2_y;

	denom = s10_x * s32_y - s32_x * s10_y;
	t_numer = s32_x * s02_y - s32_y * s02_x;
	t = t_numer / denom;
	
	v2Output.x = p0_x + (t * s10_x);	
	v2Output.y = p0_y + (t * s10_y);

	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// IsIntersecting

BtBool MtLine::IsIntersecting( MtVector2 l1p1, MtVector2 l1p2, MtVector2 l2p1, MtVector2 l2p2)
{
	BtU32 test1_a, test1_b, test2_a, test2_b;

	test1_a = CheckTriClockDir(l1p1, l1p2, l2p1);
	test1_b = CheckTriClockDir(l1p1, l1p2, l2p2);
	
	if( test1_a != test1_b )
	{
		test2_a = CheckTriClockDir(l2p1, l2p2, l1p1);
		test2_b = CheckTriClockDir(l2p1, l2p2, l1p2);
		if (test2_a != test2_b)
		{
			return BtTrue;
		}
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IntersectLineCircle

BtBool MtLine::IntersectLineCircle( const MtVector2& c,      // center
									BtFloat r,               // radius
									MtLine& line )			 // line
{
	MtVector2 dir = line.point2 - line.point1;
	MtVector2 diff = c - line.point1;
	
	BtFloat t = MtVector2::DotProduct( diff, dir ) / MtVector2::DotProduct( dir, dir );
	
	if(t < 0.0f )
	{
		t = 0.0f;
	}
	
	if( t > 1.0f )
	{
		t = 1.0f;
	}

	MtVector2 closest = MtVector2( line.point1 ) + ( dir * t );
	MtVector2 d = c - closest;
	float distsqr = MtVector2::DotProduct( d, d );
	return distsqr <= r * r;
}




