// Includes
#include <memory.h>
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"
#include "MtVector3.h"
#include "MtVector4.h"
#include "MtMath.h"

// Constructors
MtVector3::MtVector3( const BtFloat * floats)
{
   x = floats[0];
   y = floats[1];
   z = floats[2];
}

MtVector3::MtVector3( const MtVector3& myvector)
{
   x = myvector.x;
   y = myvector.y;
   z = myvector.z;
}

MtVector3::MtVector3( const MtVector2& myvector, BtFloat new_z)
{
	x = myvector.x;
	y = myvector.y;
	z = new_z;
}

MtVector3::MtVector3( BtFloat nx, BtFloat ny, BtFloat nz )
{
   x = nx;
   y = ny;
   z = nz;
}

MtVector3 MtVector3::ZERO = MtVector3( 0, 0, 0 );

MtVector3::operator MtVector4()
{
   return MtVector4(x, y, z, 1);
}

MtVector3::operator MtVector2()
{
   return MtVector2(x, y);
}

// Assignment operators
MtVector3& MtVector3::operator += ( const MtVector3& myvector)
{
   x+=myvector.x;
   y+=myvector.y;
   z+=myvector.z;
   return *this;
}

MtVector3& MtVector3::operator -= ( const MtVector3& myvector)
{
   x-=myvector.x;
   y-=myvector.y;
   z-=myvector.z;
   return *this;
}

MtVector3& MtVector3::operator *= ( BtFloat scale )
{
   x*=scale;
   y*=scale;
   z*=scale;
   return *this;
}

MtVector3& MtVector3::operator *= ( const MtQuaternion& quaternion)
{
   BtFloat rw =  -(x*-quaternion.x + y*-quaternion.y  + z*-quaternion.z);
   BtFloat rx =   (y*-quaternion.z - z*-quaternion.y) + x* quaternion.w;
   BtFloat ry =   (z*-quaternion.x - x*-quaternion.z) + y* quaternion.w;
   BtFloat rz =   (x*-quaternion.y - y*-quaternion.x) + z* quaternion.w;

   // Quaternion multiply of Q and (0, v)Q* (as calculated above)  
   x = (quaternion.y*rz - quaternion.z*ry) + quaternion.w*rx + quaternion.x*rw;
   y = (quaternion.z*rx - quaternion.x*rz) + quaternion.w*ry + quaternion.y*rw;
   z = (quaternion.x*ry - quaternion.y*rx) + quaternion.w*rz + quaternion.z*rw;

   return *this;
}

MtVector3& MtVector3::operator *= ( const MtMatrix4& matrix )
{
	BtFloat x0 = x;
	BtFloat y0 = y;
	BtFloat z0 = z;

	x = x0 * matrix.m[0][0] + y0 * matrix.m[1][0] + z0 * matrix.m[2][0] + matrix.m[3][0];
	y = x0 * matrix.m[0][1] + y0 * matrix.m[1][1] + z0 * matrix.m[2][1] + matrix.m[3][1];
	z = x0 * matrix.m[0][2] + y0 * matrix.m[1][2] + z0 * matrix.m[2][2] + matrix.m[3][2];
	return *this;
}

MtVector3& MtVector3::operator *= ( const MtMatrix3& matrix )
{
	BtFloat x0 = x;
	BtFloat y0 = y;
	BtFloat z0 = z;

	x = x0 * matrix.m[0][0] + y0 * matrix.m[1][0] + z0 * matrix.m[2][0];
	y = x0 * matrix.m[0][1] + y0 * matrix.m[1][1] + z0 * matrix.m[2][1];
	z = x0 * matrix.m[0][2] + y0 * matrix.m[1][2] + z0 * matrix.m[2][2];
	return *this;
}

MtVector3& MtVector3::operator /= ( BtFloat scale )
{
   x/=scale;
   y/=scale;
   z/=scale;
   return *this;
}

// Unary operators
MtVector3 MtVector3::operator + () const
{
   MtVector3 n;
   n.x = x;
   n.y = y;
   n.z = z;
   return n;
}

MtVector3 MtVector3::operator - () const
{
   MtVector3 n;
   n.x = -x;
   n.y = -y;
   n.z = -z;
   return n;
}

MtVector3 MtVector3::operator + ( const MtVector3& myvector) const
{
    MtVector3 n;

    n.x = x + myvector.x;
    n.y = y + myvector.y;
    n.z = z + myvector.z;
    return n;
}

MtVector3 MtVector3::operator - ( const MtVector3& myvector) const
{
    MtVector3 n;

    n.x = x - myvector.x;
    n.y = y - myvector.y;
    n.z = z - myvector.z;
    return n;
}

MtVector3 MtVector3::operator ^ ( const MtVector3& v2) const
{
    MtVector3 result;

    result.x = y * v2.z - z * v2.y;
    result.y = z * v2.x - x * v2.z;
    result.z = x * v2.y - y * v2.x;

    return result;
}

MtVector3 MtVector3::operator * ( BtFloat scale ) const
{
    MtVector3 n;

    n.x = x * scale;
    n.y = y * scale;
    n.z = z * scale;
    return n;
}

MtVector3 MtVector3::operator * ( const MtQuaternion& quaternion) const
{
   BtFloat rw =  -(x*-quaternion.x + y*-quaternion.y  + z*-quaternion.z);
   BtFloat rx =   (y*-quaternion.z - z*-quaternion.y) + x* quaternion.w;
   BtFloat ry =   (z*-quaternion.x - x*-quaternion.z) + y* quaternion.w;
   BtFloat rz =   (x*-quaternion.y - y*-quaternion.x) + z* quaternion.w;

   BtFloat nx, ny, nz;

   // Quaternion multiply of Q and (0, v)Q* (as calculated above)  
   nx = (quaternion.y*rz - quaternion.z*ry) + quaternion.w*rx + quaternion.x*rw;
   ny = (quaternion.z*rx - quaternion.x*rz) + quaternion.w*ry + quaternion.y*rw;
   nz = (quaternion.x*ry - quaternion.y*rx) + quaternion.w*rz + quaternion.z*rw;

   return MtVector3(nx, ny, nz);
}

MtVector3 MtVector3::operator * ( const MtMatrix4& matrix )
{
	MtVector3 v;
	
	v.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + matrix.m[3][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + matrix.m[3][1];
	v.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + matrix.m[3][2];
	return v;
}

MtVector3 MtVector3::operator * ( const MtMatrix3& matrix )
{
	MtVector3 v;
	
	v.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1];
	v.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2];
	
	return v;
}

MtVector3 MtVector3::operator * ( const MtMatrix4& matrix ) const
{
	MtVector3 v;
	
	v.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + matrix.m[3][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + matrix.m[3][1];
	v.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + matrix.m[3][2];
	
	return v;
}

MtVector3 MtVector3::operator * ( const MtMatrix3& matrix ) const
{
	MtVector3 v;
	
	v.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0];
	v.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1];
	v.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2];
	
	return v;
}

MtVector3 MtVector3::operator / ( BtFloat scale ) const
{
    MtVector3 n;

    n.x = x / scale;
    n.y = y / scale;
    n.z = z / scale;
    return n;
}

MtVector3 MtVector3::operator * ( const MtVector3& v3 ) const
{
	MtVector3 n;

    n.x = x * v3.x;
	n.y = y * v3.y;
	n.z = z * v3.z;

	return n;
}

// Static functions

MtVector3 MtVector3::Cross(const MtVector3& v3A, const MtVector3& v3B)
{
	MtVector3 result;

	result.x = v3A.y * v3B.z - v3A.z * v3B.y;
	result.y = v3A.z * v3B.x - v3A.x * v3B.z;
	result.z = v3A.x * v3B.y - v3A.y * v3B.x;

	return result;
}

MtVector3 MtVector3::CrossProduct(const MtVector3& v3A, const MtVector3& v3B)
{
	MtVector3 result;

	result.x = v3A.y * v3B.z - v3A.z * v3B.y;
	result.y = v3A.z * v3B.x - v3A.x * v3B.z;
	result.z = v3A.x * v3B.y - v3A.y * v3B.x;

	return result;
}

BtFloat MtVector3::DotProduct (const MtVector3& v2) const
{
   return x*v2.x + y*v2.y + z*v2.z;
}

BtFloat MtVector3::Dot(const MtVector3& v3A, const MtVector3& v3B)
{
	return v3A.DotProduct( v3B );
}

BtFloat MtVector3::DotProduct(const MtVector3& v3A, const MtVector3& v3B)
{
	return v3A.DotProduct( v3B );
}

// Comparisons
BtBool MtVector3::operator == ( const MtVector3& myvector ) const
{
    BtBool t = (
       (fabs(myvector.x - x) < MtFloatingPointTolerance ) &&
       (fabs(myvector.y - y) < MtFloatingPointTolerance ) &&
       (fabs(myvector.z - z) < MtFloatingPointTolerance )
           );

    return t;
}

BtBool MtVector3::operator != ( const MtVector3& myvector) const
{
    return (memcmp((void*) this, (void*) &myvector, sizeof(MtVector3)) != 0);
}

// Public functions
BtFloat MtVector3::GetLengthSquared()
{
   return x*x + y*y + z*z;
}

MtVector3 MtVector3::GetNormalize() const
{
   MtVector3 result;

   BtFloat magnitude = Magnitude();

   if( magnitude != 0.0f )
   {
       BtFloat one_over_magnitude = 1.0f / magnitude;

       result.x = x * one_over_magnitude;
       result.y = y * one_over_magnitude;
       result.z = z * one_over_magnitude;
   }
   return result;
}

MtVector3 MtVector3::Normalise()
{
	return Normalize();
}

MtVector3 MtVector3::Normalize()
{
   BtFloat magnitude = Magnitude();

   if( magnitude != 0.0f )
   {
       BtFloat one_over_magnitude = 1.0f / magnitude;

       x *= one_over_magnitude;
       y *= one_over_magnitude;
       z *= one_over_magnitude;
   }
   return *this;
}

MtVector3 MtVector3::GetNormalise() const
{
   return GetNormalize();
}

MtVector3 MtVector3::CrossProduct (const MtVector3& v2) const
{
    MtVector3 result;

    result.x = y * v2.z - z * v2.y;
    result.y = z * v2.x - x * v2.z;
    result.z = x * v2.y - y * v2.x;

    return result;
}

MtVector3 MtVector3::Min (const MtVector3 &v1)
{ 
	MtVector3 result;
	result.x = MtMin(x, v1.x);
	result.y = MtMin(y, v1.y);
	result.z = MtMin(z, v1.z);
	return result;
}

MtVector3 MtVector3::Max (const MtVector3 &v1)
{ 
	MtVector3 result;
	result.x = MtMax(x, v1.x);
	result.y = MtMax(y, v1.y);
	result.z = MtMax(z, v1.z);
	return result;
}

MtVector3 MtVector3::MidPoint(MtVector3 &p1, MtVector3 &p2)
{
	MtVector3 p;

	p.x = (p1.x + p2.x) * 0.5f;
	p.y = (p1.y + p2.y) * 0.5f;
	p.z = (p1.z + p2.z) * 0.5f;

	return p;
}

MtVector3 operator*(MtMatrix3 &matrix, MtVector3 &v)
{
	MtVector3 result;

	result.x = v.x * matrix.m[0][0] + v.y * matrix.m[1][0] + v.z * matrix.m[2][0];
	result.y = v.x * matrix.m[0][1] + v.y * matrix.m[1][1] + v.z * matrix.m[2][1];
	result.z = v.x * matrix.m[0][2] + v.y * matrix.m[1][2] + v.z * matrix.m[2][2];

	return result;
}

MtVector3 operator*(MtVector3 &v, MtMatrix3 &matrix)
{
	MtVector3 result;

	result.x = v.x * matrix.m[0][0] + v.y * matrix.m[1][0] + v.z * matrix.m[2][0];
	result.y = v.x * matrix.m[0][1] + v.y * matrix.m[1][1] + v.z * matrix.m[2][1];
	result.z = v.x * matrix.m[0][2] + v.y * matrix.m[1][2] + v.z * matrix.m[2][2];

	return result;
}

BtFloat MtVector3::GetLength() const
{
	return sqrtf(x*x + y*y + z*z);
}

BtFloat MtVector3::Magnitude() const
{
	return sqrtf(x*x + y*y + z*z);
}