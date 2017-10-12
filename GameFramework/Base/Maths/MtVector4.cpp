// Library includes ///////////////////////////////////////////////////////////
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"
#include "MtVector3.h"
#include "MtVector4.h"
#include "MtMath.h"

// Constructors ///////////////////////////////////////////////////////////////
MtVector4::MtVector4( const BtFloat* floats)
{
   x = floats[0];
   y = floats[1];
   z = floats[2];
   w = floats[3];
}

MtVector4::MtVector4( const MtVector4& myvector )
{
   x = myvector.x;
   y = myvector.y;
   z = myvector.z;
   w = myvector.w;
}

MtVector4::MtVector4( const MtVector3& myvector )
{
   x = myvector.x;
   y = myvector.y;
   z = myvector.z;
   w = 1.0f;
}

MtVector4::MtVector4( const MtVector3& myvector, BtFloat w )
{
    x = myvector.x;
    y = myvector.y;
    z = myvector.z;
    this->w = w;
}

MtVector4::MtVector4( BtFloat nx, BtFloat ny, BtFloat nz, BtFloat nw )
{
   x = nx;
   y = ny;
   z = nz;
   w = nw;
}

// Casting
MtVector4::operator BtFloat* ()
{
   return &x;
}

MtVector4::operator const BtFloat* () const
{
   return &x;
}

MtVector4::operator MtVector3()
{
   return MtVector3(x, y, z);
}

// Maths operators/////////////////////////////////////////////////////////////
MtVector4 MtVector4::operator * ( const MtVector4& v2 )
{
	MtVector4 result;

    result.x = x * v2.x;
	result.y = y * v2.y;
	result.z = z * v2.z;
	result.w = w * v2.w;

	return result;
}

MtVector4 MtVector4::operator * ( const MtMatrix4& matrix ) const
{
	MtVector4 v;
	
	v.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + w * matrix.m[3][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + w * matrix.m[3][1];
	v.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + w * matrix.m[3][2];
	v.w = x * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + w * matrix.m[3][3];
	
	return v;
}


MtVector4 MtVector4::operator *= ( BtFloat scale )
{
	x*=scale;
	y*=scale;
	z*=scale;
	return *this;
}

MtVector4 MtVector4::operator * ( BtFloat fValue ) const
{
	MtVector4 v;

	v.x = x * fValue;
	v.y = y * fValue;
	v.z = z * fValue;
	v.w = w * fValue;

	return v;
}

MtVector4 MtVector4::operator + ( const MtVector4& myvector) const
{
    MtVector4 n;

    n.x = x + myvector.x;
    n.y = y + myvector.y;
    n.z = z + myvector.z;
	n.w = w + myvector.w;
    return n;
}

MtVector4 MtVector4::operator - ( const MtVector4& myvector) const
{
    MtVector4 n;

    n.x = x - myvector.x;
    n.y = y - myvector.y;
    n.z = z - myvector.z;
	n.w = w - myvector.w;
    return n;
}

MtVector4 MtVector4::operator - () const
{
	MtVector4 n;
	n.x = -x;
	n.y = -y;
	n.z = -z;
	n.w = -w;
	return n;
}
