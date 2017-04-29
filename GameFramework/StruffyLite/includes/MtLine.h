////////////////////////////////////////////////////////////////////////////////
// MtLine.h

// Include guard
#pragma once
#include "MtVector2.h"
#include "MtVector3.h"
#include "MtTriangle.h"
#include "MtPlane.h"

// Class definition
struct MtLine
{
       MtLine()
       {
          point1 = point2 = MtVector3(0, 0, 0);
       }

       MtLine(MtVector3 a, MtVector3 b)
       {
          point1 = a;
          point2 = b;
       }
		
	   // Public functions
	   void				FindLineRadius();
	   BtBool			Collide( const MtTriangle &triangle, MtVector3 &intersection );
	   BtBool			CollideWithPlane( const MtPlane &plane, MtVector3 &intersection );
	   static BtBool	GetLineIntersection(MtVector2 v2PointA, MtVector2 v2PointB, MtVector2 v2PointC, MtVector2 v2PointD, MtVector2 &v2Output);

	   static BtBool	IntersectLineCircle( const MtVector2& c,	// center
										     BtFloat r,             // radius
										     MtLine& line );	    // segment end

	   static BtBool	IsIntersecting(MtVector2 l1p1, MtVector2 l1p2, MtVector2 l2p1, MtVector2 l2p2);

	   // Member variables
       MtVector3		point1;
       MtVector3		point2;
       MtVector3		center;
       BtFloat			m_fRadius;
};
