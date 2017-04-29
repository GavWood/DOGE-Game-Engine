// Local includes
#include "MtVector3.h"
#include "MtPlane.h"
#include "MtPoint.h"

// Class definition
BtFloat MtPoint::SideOfPlane( const MtPlane &v3Plane )
{
   return v3Plane.DistanceTo( m_v3Point );
}

BtFloat MtPoint::SideOfPlane( const MtVector3 &v3Point, const MtPlane &v3Plane )
{
   return v3Plane.DistanceTo( v3Point );
}
