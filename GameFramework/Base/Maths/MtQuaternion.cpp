////////////////////////////////////////////////////////////////////////////////
// http://www.gamasutra.com/features/19980703/quaternions_01.htm

// Standard includes
#include <math.h>

// Local includes
#include "MtQuaternion.h"
#include "MtVector3.h"
#include "MtMatrix3.h"
#include "MtMath.h"

// Constructors ///////////////////////////////////////////////////////////////
MtQuaternion::MtQuaternion()
{
}

MtQuaternion::MtQuaternion(const MtMatrix4 &mat)
{
	*this = MtQuaternion(mat.GetRotation());
}

MtQuaternion::MtQuaternion(const MtMatrix3 &mat)
{	
    BtU32 nxt[3] = {1, 2, 0};
	BtU32 i, j, k;

	// Determine the trace of the matrix
	BtFloat trace = mat._00 + mat._11 + mat._22;
	BtFloat s;
	BtFloat q[4];

	if( trace > 0.0f )
	{
		s = (BtFloat)sqrt(trace + 1.0f);
		w = s * 0.5f;
		s = 0.5f / s;
		x = (mat._12 - mat._21) * s;
		y = (mat._20 - mat._02) * s;
		z = (mat._01 - mat._10) * s;
	}
	else
	{
		// diagonal is negative
		i = 0;
		if (mat[1][1] > mat[0][0])
		{	
			i = 1;
		}

		if (mat[2][2] > mat[i][i])
		{
			i = 2;
		}
          
		j = nxt[i];
        k = nxt[j];

		s = (BtFloat)sqrt((mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f);
      
		q[i] = s * 0.5f;
            
		if( s != 0.0 )
		{
			s = 0.5f / s;
		}

		q[3] = (mat[j][k] - mat[k][j]) * s;
		q[j] = (mat[i][j] + mat[j][i]) * s;
		q[k] = (mat[i][k] + mat[k][i]) * s;

		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	}
}

MtQuaternion::MtQuaternion( const MtQuaternion &q )
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

BtFloat MtQuaternion::DotProduct( const MtQuaternion& v2 ) const
{
	return x*v2.x + y*v2.y + z*v2.z + w*v2.w;
}

MtQuaternion MtQuaternion::operator - () const
{
	MtQuaternion n;
	n.x = -x;
	n.y = -y;
	n.z = -z;
	n.w = -w;
	return n;
}

////////////////////////////////////////////////////////////////////////////////
// Spherical linear interpolation

//static
MtQuaternion MtQuaternion::SLerp( BtFloat t, MtQuaternion& From, MtQuaternion& To )
{
	MtQuaternion To1;

	BtFloat Cosom = From.DotProduct( To );

	if( Cosom < 0.0f )
	{
		Cosom = -Cosom;

		To1 = -To;
	}
	else if( From == To )
	{
		// Equal case, early exit

		return To;
	}
	else
	{
		To1 = To;
	}

	BtFloat	Scale0, Scale1;

	if ( ( 1.0f - Cosom ) > 0.0001f )
	{
		BtFloat Omega = MtACos( Cosom );
		BtFloat Sinom = MtSin( Omega );

		Scale0 = MtSin( ( 1.0f - t ) * Omega ) / Sinom;
		Scale1 = MtSin( t * Omega ) / Sinom;
	}
	else
	{
		Scale0 = 1.0f - t;
		Scale1 = t;
	}

    MtQuaternion result;
    result = MtQuaternion( From * Scale0 ) + MtQuaternion( To1 * Scale1 );
    return result;
}

// Comparison functions ///////////////////////////////////////////////////////
BtS32 MtQuaternion::operator==(const MtQuaternion &op2) const
{
	return (op2.x == x) && (op2.y == y) && (op2.z == z) && (op2.w == w);
}

// Casting ////////////////////////////////////////////////////////////////////
MtQuaternion::operator MtMatrix3()
{
	// Make a const quaternion
	const MtQuaternion quat = *this;

	// Make a matrix from this quaternion
	return MtMatrix3( quat );
}

MtQuaternion::operator BtFloat* ()
{
   return &x;
}

MtQuaternion::operator const BtFloat* () const
{
   return &x;
}

// Maths functions ////////////////////////////////////////////////////////////
MtQuaternion &MtQuaternion::operator*=(const BtFloat &op2)
{
	x *= op2;
	y *= op2;
	z *= op2;
	w *= op2;
	   
	return *this;
}

MtQuaternion &MtQuaternion::operator*=(const MtQuaternion &op2)
{
    x = (y * op2.z) - (z * op2.y) + (w * op2.x) + (x * op2.w);
    y = (z * op2.x) - (x * op2.z) + (w * op2.y) + (y * op2.w);
    z = (x * op2.y) - (y * op2.x) + (w * op2.z) + (z * op2.w);
    w = (w * op2.w) - (x * op2.x) - (y * op2.y) - (z * op2.z);

    return *this;
}

MtQuaternion &MtQuaternion::operator+=(const MtQuaternion &op2)
{
	x+=op2.x;
	y+=op2.y;
	z+=op2.z;
	w+=op2.w;

	return *this;
}

MtQuaternion MtQuaternion::operator+(const MtQuaternion &op1)
{
	MtQuaternion q;

	q = *this;

	q.x += op1.x;
	q.y += op1.y;
	q.z += op1.z;
	q.w += op1.w;
	   
	return q;
}

MtQuaternion operator*(MtQuaternion &q, BtFloat s)
{
	MtQuaternion r;

	r.x = q.x * s;
	r.y = q.y * s;
	r.z = q.z * s;
	r.w = q.w * s;

	return r;
}

MtQuaternion operator*(BtFloat s, MtQuaternion &q)
{
	MtQuaternion r;

	r.x = q.x * s;
	r.y = q.y * s;
	r.z = q.z * s;
	r.w = q.w * s;

	return r;
}

MtQuaternion operator*(MtQuaternion &op1, MtQuaternion &op2)
{
	MtQuaternion q;

    q.x = (op1.y * op2.z) - (op1.z * op2.y) + (op1.w * op2.x) + (op1.x * op2.w);
    q.y = (op1.z * op2.x) - (op1.x * op2.z) + (op1.w * op2.y) + (op1.y * op2.w);
    q.z = (op1.x * op2.y) - (op1.y * op2.x) + (op1.w * op2.z) + (op1.z * op2.w);
    q.w = (op1.w * op2.w) - (op1.x * op2.x) - (op1.y * op2.y) - (op1.z * op2.z);

    return q;
}

// Public functions ///////////////////////////////////////////////////////////
void MtQuaternion::SetIdentity()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

MtQuaternion &MtQuaternion::Normalise()
{
    const BtFloat oo_magnitude = 1.0f / sqrtf ( (x * x) + (y * y) + (z * z) + (w * w) );

    x *= oo_magnitude;
    y *= oo_magnitude;
    z *= oo_magnitude;
    w *= oo_magnitude;

	return *this;
}

MtQuaternion operator*(MtQuaternion &q, MtVector3 &v)
{
	MtQuaternion result;

	result.w = -(q.x * v.x + q.y * v.y + q.z * v.z);
	result.x =   q.w * v.x + q.y * v.z - q.z * v.y;
	result.y =   q.w * v.y + q.z * v.x - q.x * v.z,
	result.z =   q.w * v.z + q.x * v.y - q.y * v.x;
	return result;
}

MtQuaternion operator*(MtVector3 &v, MtQuaternion &q)
{
	MtQuaternion result;

	result.w = -(q.x * v.x + q.y * v.y + q.z * v.z);
	result.x =   q.w * v.x + q.z * v.y - q.y * v.z;
	result.y =   q.w * v.y + q.x * v.z - q.z * v.x,
	result.z =   q.w * v.z + q.y * v.x - q.x * v.y;
	return result;
}

