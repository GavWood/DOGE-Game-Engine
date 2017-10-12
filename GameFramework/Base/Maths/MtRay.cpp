// Local includes /////////////////////////////////////////////////////////////
#include "MtVector3.h"
#include "MtVector2.h"
#include "MtVector4.h"
#include "MtRay.h"
#include "MtTriangle.h"
#include "MtPlane.h"

BtBool MtRay::Collide(const MtTriangle &triangle, MtVector3 &intersection)
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

BtBool MtRay::CollideWithPlane(const MtPlane &plane, MtVector3 &intersection)
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

   intersection = MtVector3 ( point1.x + i * t, point1.y + j * t, point1.z + k * t );

   return BtTrue;
}
