// Maths
#include "MtVector2.h"

// Library includes
#include "MtQuaternion.h"
#include "MtMatrix4.h"

// Constructors
MtVector2::MtVector2( const BtFloat * floats)
{
   x = floats[0];
   y = floats[1];
}

MtVector2::MtVector2( const MtVector2& myvector)
{
   x = myvector.x;
   y = myvector.y;
}

MtVector2::MtVector2( BtFloat nx, BtFloat ny )
{
   x = nx;
   y = ny;
}

// Casting
MtVector2::operator BtFloat* ()
{
   return &x;
}

MtVector2::operator const BtFloat* () const
{
   return &x;
}

// Maths operators
MtVector2& MtVector2::operator += ( const MtVector2& myvector)
{
   x+=myvector.x;
   y+=myvector.y;
   return *this;
}

MtVector2& MtVector2::operator -= ( const MtVector2& myvector)
{
   x-=myvector.x;
   y-=myvector.y;
   return *this;
}

MtVector2 MtVector2::operator - () const
{
	MtVector2 n;
	n.x = -x;
	n.y = -y;
	return n;
}

MtVector2& MtVector2::operator *= ( BtFloat scale )
{
   x*=scale;
   y*=scale;
   return *this;
}

MtVector2& MtVector2::operator /= ( BtFloat scale )
{
   x/=scale;
   y/=scale;
   return *this;
}

MtVector2 MtVector2::operator * ( BtFloat scale ) const
{
    MtVector2 n;

    n.x = x * scale;
    n.y = y * scale;
    return n;
}

MtVector2 MtVector2::operator / ( BtFloat scale ) const
{
    MtVector2 n;

    n.x = x / scale;
    n.y = y / scale;
    return n;
}

MtVector2 MtVector2::operator * (  const MtVector2& myvector ) const
{
    MtVector2 n;

    n.x = x * myvector.x;
    n.y = y * myvector.y;
    return n;
}

MtVector2 MtVector2::operator * ( const MtMatrix4& matrix ) const
{
	MtVector2 v;
	
	v.x = x * matrix.m[0][0] + y * matrix.m[1][0] + matrix.m[3][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1] + matrix.m[3][1];
	return v;
}

MtVector2 MtVector2::operator * ( const MtMatrix3& matrix ) const
{
	MtVector2 v;

	v.x = x * matrix.m[0][0] + y * matrix.m[1][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1];
	return v;
}

MtVector2 MtVector2::operator + ( const MtVector2& myvector) const
{
    MtVector2 n;

    n.x = x + myvector.x;
    n.y = y + myvector.y;
    return n;
}

MtVector2 MtVector2::operator - ( const MtVector2& myvector) const
{
    MtVector2 n;

    n.x = x - myvector.x;
    n.y = y - myvector.y;
    return n;
}

BtFloat MtVector2::Magnitude() const
{
   return (BtFloat) sqrt( x*x + y*y );
}

MtVector2& MtVector2::Normalize()
{
   BtFloat magnitude = Magnitude();

   if( magnitude )
   {
       BtFloat one_over_magnitude = 1.0f / magnitude;

       x *= one_over_magnitude;
       y *= one_over_magnitude;
   }
   return *this;
}

MtVector2 MtVector2::GetNormalize()
{
	BtFloat magnitude = Magnitude();

	MtVector2 result;

	if( magnitude )
	{
		BtFloat one_over_magnitude = 1.0f / magnitude;

		result = MtVector2( x * one_over_magnitude, y * one_over_magnitude );
	}
	return result;
}

BtFloat MtVector2::CrossProduct( const MtVector2& v2 ) const
{
	return x*v2.y - y*v2.x;
}

BtFloat MtVector2::DotProduct( const MtVector2& v2 ) const
{
   return x*v2.x + y*v2.y;
}

BtFloat MtVector2::Dot(const MtVector2& v2A, const MtVector2& v2B)
{
	return v2A.DotProduct( v2B );
}

BtFloat MtVector2::DotProduct(const MtVector2& v2A, const MtVector2& v2B)
{
	return v2A.DotProduct( v2B );
}

MtVector2 MtVector2::Reflect( const MtVector2& v2Velocity, const MtVector2& v2Normal )
{
	return v2Velocity - ( v2Normal * v2Normal.DotProduct( v2Velocity ) * 2.0f );
}