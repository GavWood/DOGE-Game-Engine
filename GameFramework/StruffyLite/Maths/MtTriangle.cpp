// Local includes /////////////////////////////////////////////////////////////
#include "MtMath.h"
#include "MtVector3.h"
#include "MtVector4.h"
#include "MtLine.h"
#include "MtTriangle.h"
#include "MtPlane.h"

// Class definition ///////////////////////////////////////////////////////////
BtBool MtTriangle::SphericalTest(const MtTriangle &other)
{
  BtFloat temp_dist;

  BtFloat t = (radius + other.radius);
  temp_dist = t * t;

  if(( ((other.center.x-center.x) * (other.center.x-center.x))+
       ((other.center.y-center.y) * (other.center.y-center.y))+
       ((other.center.z-center.z) * (other.center.z-center.z))
     ) < temp_dist )
  {
      return BtTrue;
  }

	return false;
}

void MtTriangle::FindPolygonCenter()
{
     BtFloat minx,miny,minz,maxx,maxy,maxz;
     minx = maxx = a.x;
     miny = maxy = a.y;
     minz = maxz = a.z;

     if (b.x < minx)
       minx = b.x;

     if (c.x < minx)
       minx = c.x;

     if (b.y < miny)
       miny = b.y;

     if (c.y < miny)
       miny = c.y;

     if (b.z < minz)
       minz = b.z;

     if (c.z < minz)
       minz = c.z;

     if (b.x > maxx)
       maxx = b.x;

     if (c.x > maxx)
       maxx = c.x;

     if (b.y > maxy)
       maxy = b.y;

     if (c.y > maxy)
       maxy = c.y;

     if (b.z > maxz)
       maxz = b.z;

     if (c.z > maxz)
       maxz = c.z;

     center.x = ( minx + maxx ) * 0.5f;
     center.y = ( miny + maxy ) * 0.5f;
     center.z = ( minz + maxz ) * 0.5f;
}

void MtTriangle::FindPolygonRadius()
{
     BtFloat temp_radius;

     temp_radius = (BtFloat) sqrt(((center.x-a.x)*(center.x-a.x))+
				                ((center.y-a.y)*(center.y-a.y))+
				                ((center.z-a.z)*(center.z-a.z)));

     if ((sqrt(((center.x-b.x)*(center.x-b.x))+
	           ((center.y-b.y)*(center.y-b.y))+
	           ((center.z-b.z)*(center.z-b.z)))) > temp_radius)

     temp_radius = (BtFloat) sqrt(((center.x-b.x)*(center.x-b.x))+
				                ((center.y-b.y)*(center.y-b.y))+
				                ((center.z-b.z)*(center.z-b.z)));

     if (((BtFloat) sqrt(((center.x-c.x)*(center.x-c.x))+
		               ((center.y-c.y)*(center.y-c.y))+
		               ((center.z-c.z)*(center.z-c.z))) > temp_radius))

     temp_radius = (BtFloat) sqrt(((center.x-c.x)*(center.x-c.x))+
				                ((center.y-c.y)*(center.y-c.y))+
				                ((center.z-c.z)*(center.z-c.z)));

     radius = temp_radius;
}

void MtTriangle::FindNormal()
{
	normal = MtVector3::CrossProduct( b - a, c - a );
	normal.Normalise();
}

MtTriangle& MtTriangle::operator *= ( const MtMatrix4& matrix )
{
	a = a * matrix;
	b = b * matrix;
	c = c * matrix;
	return *this;
}

BtU32 MtTriangle::Collide(MtTriangle &tri2, MtVector3 intersections[6] )
{
   MtLine line1 = MtLine( a, b );
   MtLine line2 = MtLine( b, c );
   MtLine line3 = MtLine( a, c );

   MtLine line4 = MtLine( tri2.a, tri2.b );
   MtLine line5 = MtLine( tri2.b, tri2.c );
   MtLine line6 = MtLine( tri2.a, tri2.c );

   BtU32 count = 0;

   if( line1.Collide( tri2, intersections[count] ) )
   {
		count++;
   }

   if( line2.Collide( tri2, intersections[count] ) )
   {
		count++;
   }

   if( line3.Collide( tri2, intersections[count] ) )
   {
		count++;
   }

   if( line4.Collide( *this, intersections[count] ) )
   {
		count++;
   }

   if( line5.Collide( *this, intersections[count] ) )
   {
		count++;
   }

   if( line6.Collide( *this, intersections[count] ) )
   {
		count++;
   }

   return count;
}

//----------------------------------------------------------------------------
BtFloat MtTriangle::SqrDistance( const MtVector3 &v3Point )
{
    MtVector3 kDiff  = a - v3Point;
    MtVector3 kEdge0 = b - a;
    MtVector3 kEdge1 = c - a;
    BtFloat fA00 = kEdge0.GetLengthSquared();
    BtFloat fA01 = kEdge0.DotProduct(kEdge1);
    BtFloat fA11 = kEdge1.GetLengthSquared();
    BtFloat fB0 = kDiff.DotProduct(kEdge0);
    BtFloat fB1 = kDiff.DotProduct(kEdge1);
    BtFloat fC = kDiff.GetLengthSquared();
    BtFloat fDet = MtAbs(fA00*fA11-fA01*fA01);
    BtFloat fS = fA01*fB1-fA11*fB0;
    BtFloat fT = fA01*fB0-fA00*fB1;
    BtFloat fSqrDistance;

    if (fS + fT <= fDet)
    {
        if (fS < (BtFloat)0.0)
        {
            if (fT < (BtFloat)0.0)  // region 4
            {
                if (fB0 < (BtFloat)0.0)
                {
                    fT = (BtFloat)0.0;
                    if (-fB0 >= fA00)
                    {
                        fS = (BtFloat)1.0;
                        fSqrDistance = fA00+((BtFloat)2.0)*fB0+fC;
                    }
                    else
                    {
                        fS = -fB0/fA00;
                        fSqrDistance = fB0*fS+fC;
                    }
                }
                else
                {
                    fS = (BtFloat)0.0;
                    if (fB1 >= (BtFloat)0.0)
                    {
                        fT = (BtFloat)0.0;
                        fSqrDistance = fC;
                    }
                    else if (-fB1 >= fA11)
                    {
                        fT = (BtFloat)1.0;
                        fSqrDistance = fA11+((BtFloat)2.0)*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDistance = fB1*fT+fC;
                    }
                }
            }
            else  // region 3
            {
                fS = (BtFloat)0.0;
                if (fB1 >= (BtFloat)0.0)
                {
                    fT = (BtFloat)0.0;
                    fSqrDistance = fC;
                }
                else if (-fB1 >= fA11)
                {
                    fT = (BtFloat)1.0;
                    fSqrDistance = fA11+((BtFloat)2.0)*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDistance = fB1*fT+fC;
                }
            }
        }
        else if (fT < (BtFloat)0.0)  // region 5
        {
            fT = (BtFloat)0.0;
            if (fB0 >= (BtFloat)0.0)
            {
                fS = (BtFloat)0.0;
                fSqrDistance = fC;
            }
            else if (-fB0 >= fA00)
            {
                fS = (BtFloat)1.0;
                fSqrDistance = fA00+((BtFloat)2.0)*fB0+fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDistance = fB0*fS+fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            BtFloat fInvDet = ((BtFloat)1.0)/fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDistance = fS*(fA00*fS+fA01*fT+((BtFloat)2.0)*fB0) +
                fT*(fA01*fS+fA11*fT+((BtFloat)2.0)*fB1)+fC;
        }
    }
    else
    {
        BtFloat fTmp0, fTmp1, fNumer, fDenom;

        if (fS < (BtFloat)0.0)  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0f*fA01+fA11;
                if (fNumer >= fDenom)
                {
                    fS = (BtFloat)1.0;
                    fT = (BtFloat)0.0;
                    fSqrDistance = fA00+((BtFloat)2.0)*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = (BtFloat)1.0 - fS;
                    fSqrDistance = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                        fT*(fA01*fS+fA11*fT+((BtFloat)2.0)*fB1)+fC;
                }
            }
            else
            {
                fS = (BtFloat)0.0;
                if (fTmp1 <= (BtFloat)0.0)
                {
                    fT = (BtFloat)1.0;
                    fSqrDistance = fA11+((BtFloat)2.0)*fB1+fC;
                }
                else if (fB1 >= (BtFloat)0.0)
                {
                    fT = (BtFloat)0.0;
                    fSqrDistance = fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDistance = fB1*fT+fC;
                }
            }
        }
        else if (fT < (BtFloat)0.0)  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-((BtFloat)2.0)*fA01+fA11;
                if (fNumer >= fDenom)
                {
                    fT = (BtFloat)1.0;
                    fS = (BtFloat)0.0;
                    fSqrDistance = fA11+((BtFloat)2.0)*fB1+fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = (BtFloat)1.0 - fT;
                    fSqrDistance = fS*(fA00*fS+fA01*fT+((BtFloat)2.0)*fB0) +
                        fT*(fA01*fS+fA11*fT+((BtFloat)2.0)*fB1)+fC;
                }
            }
            else
            {
                fT = (BtFloat)0.0;
                if (fTmp1 <= (BtFloat)0.0)
                {
                    fS = (BtFloat)1.0;
                    fSqrDistance = fA00+((BtFloat)2.0)*fB0+fC;
                }
                else if (fB0 >= (BtFloat)0.0)
                {
                    fS = (BtFloat)0.0;
                    fSqrDistance = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDistance = fB0*fS+fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if (fNumer <= (BtFloat)0.0)
            {
                fS = (BtFloat)0.0;
                fT = (BtFloat)1.0;
                fSqrDistance = fA11+((BtFloat)2.0)*fB1+fC;
            }
            else
            {
                fDenom = fA00-2.0f*fA01+fA11;
                if (fNumer >= fDenom)
                {
                    fS = (BtFloat)1.0;
                    fT = (BtFloat)0.0;
                    fSqrDistance = fA00+((BtFloat)2.0)*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = (BtFloat)1.0 - fS;
                    fSqrDistance = fS*(fA00*fS+fA01*fT+((BtFloat)2.0)*fB0) +
                        fT*(fA01*fS+fA11*fT+((BtFloat)2.0)*fB1)+fC;
                }
            }
        }
    }

    // account for numerical round-off error
    if (fSqrDistance < (BtFloat)0.0)
    {
        fSqrDistance = (BtFloat)0.0;
    }
    return fSqrDistance;
}









