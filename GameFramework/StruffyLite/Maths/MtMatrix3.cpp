////////////////////////////////////////////////////////////////////////////////
// MtMatrix3
//
// http://www.gamasutra.com/features/19980703/quaternions_01.htm

// Local includes
#include "MtMath.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"
#include "MtQuaternion.h"
#include "MtVector3.h"
#include "MtPlane.h"

// Static members /////////////////////////////////////////////////////////////
MtMatrix3 MtMatrix3::m_identity(1.0f, 0.0f, 0.0f,
							  0.0f, 1.0f, 0.0f,
							  0.0f, 0.0f, 1.0f);

MtMatrix3 MtMatrix3::m_zero(0.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 0.0f);

MtMatrix3::MtMatrix3( const MtVector3& row1, const MtVector3& row2, const MtVector3& row3 )
{
	// Row 1
	_00 = row1.x;
	_01 = row1.y;
	_02 = row1.z;

	// Row 2
	_10 = row2.x;
	_11 = row2.y;
	_12 = row2.z;

	// Row 3
	_20 = row3.x;
	_21 = row3.y;
	_22 = row3.z;
}

MtMatrix3::MtMatrix3( const MtMatrix4 &m4 )
{
	// Row 1
	_00 = m4._00;
	_01 = m4._01;
	_02 = m4._02;

	// Row 2
	_10 = m4._10;
	_11 = m4._11;
	_12 = m4._12;

	// Row 3
	_20 = m4._20;
	_21 = m4._21;
	_22 = m4._22;
}


////////////////////////////////////////////////////////////////////////////////
// Construct a matrix from a quaternion

MtMatrix3::MtMatrix3( const MtQuaternion& quat )
{
	BtFloat wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2; 

	// calculate coefficients
	x2 = quat.x + quat.x; y2 = quat.y + quat.y; 
	z2 = quat.z + quat.z;
	xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
	yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
	wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;

	m[0][0] = 1.0f - (yy + zz); m[1][0] = xy - wz;
	m[2][0] = xz + wy;

	m[0][1] = xy + wz; m[1][1] = 1.0f - (xx + zz);
	m[2][1] = yz - wx;

	m[0][2] = xz - wy; m[1][2] = yz + wx;
	m[2][2] = 1.0f - (xx + yy);
}

// Maths operations ///////////////////////////////////////////////////////////
MtMatrix3::MtMatrix3(void)
{
	_00 = 0;
	_01 = 0;
	_02 = 0;
	_10 = 0;
	_11 = 0;
	_12 = 0;
	_20 = 0;
	_21 = 0;
	_22 = 0;
}

MtMatrix3::MtMatrix3(	BtFloat r1c1, BtFloat r1c2, BtFloat r1c3, 
						BtFloat r2c1, BtFloat r2c2, BtFloat r2c3, 
						BtFloat r3c1, BtFloat r3c2, BtFloat r3c3 )
{
	this->_00 = r1c1;
	this->_01 = r1c2;
	this->_02 = r1c3;
	this->_10 = r2c1;
	this->_11 = r2c2;
	this->_12 = r2c3;
	this->_20 = r3c1;
	this->_21 = r3c2;
	this->_22 = r3c3;
}

BtFloat* MtMatrix3::operator [] (BtU32 i)
{
	return m[i];
}

const BtFloat* MtMatrix3::operator [] (BtU32 i) const
{
	return m[i];
}

void MtMatrix3::SetIdentity()
{
	_00 = 1.0f; _01 =   0;  _02 =    0;
	_10 =    0; _11 = 1.0f; _12 =    0;
	_20 =    0; _21 =    0; _22 = 1.0f;
}

BtFloat MtMatrix3::GetDeterminent() const
{
	BtFloat d = _00*_11*_22 -
		_00*_21*_12 +
		_10*_21*_02 -
		_10*_01*_22 +
		_20*_01*_12 -
		_20*_11*_02;

	if(d == 0) d = 1;

	return d;
}

void MtMatrix3::SetRotationX(BtFloat theta)
{
	BtFloat cos_theta = (BtFloat) cos(theta);
	BtFloat sin_theta = (BtFloat) sin(theta);

	SetIdentity();

	_11 =  cos_theta; _12 = sin_theta;
	_21 = -sin_theta; _22 = cos_theta;
}

void MtMatrix3::SetRotationY(BtFloat theta)
{
	BtFloat cos_theta = (BtFloat) cos(theta);
	BtFloat sin_theta = (BtFloat) sin(theta);

	SetIdentity();

	_00 = cos_theta; _02 = -sin_theta;
	_20 = sin_theta; _22 =  cos_theta;
}

void MtMatrix3::SetRotationZ(BtFloat theta)
{
	BtFloat cos_theta = (BtFloat) cos(theta);
	BtFloat sin_theta = (BtFloat) sin(theta);

	SetIdentity();

	_00 =  cos_theta; _01 = sin_theta;
	_10 = -sin_theta; _11 = cos_theta;
}

MtMatrix3 MtMatrix3::GetTranspose() const
{
	return MtMatrix3( _00,_10,_20,
		              _01,_11,_21,
					  _02,_12,_22
				    );
}

void MtMatrix3::LookAt( const MtVector3& v3Position, const MtVector3& v3LookAt, const MtVector3& v3Up )
{
	MtVector3 v3Z = v3LookAt - v3Position;
	v3Z.Normalize();

	MtVector3 v3X = MtVector3::CrossProduct( v3Up, v3Z );
	v3X.Normalize();

	MtVector3 v3Y = MtVector3::CrossProduct( v3Z, v3X );
	v3Y.Normalize();

	Row0( MtVector3( v3X.X(),  v3Y.X(),  v3Z.X() ) );
	Row1( MtVector3( v3X.Y(),  v3Y.Y(),  v3Z.Y() ) );
	Row2( MtVector3( v3X.Z(),  v3Y.Z(),  v3Z.Z() ) );
}

MtMatrix3 MtMatrix3::GetInverse() const
{
	BtFloat d = _00*_11*_22 - 
			  _00*_21*_12 + 
			  _10*_21*_02 - 
			  _10*_01*_22 + 
			  _20*_01*_12 - 
			  _20*_11*_02;

	if (d == 0) d = 1;

	return MtMatrix3(	(_11*_22-_12*_21)/d,
					   -(_01*_22-_02*_21)/d,
						(_01*_12-_02*_11)/d,
					   -(_10*_22-_12*_20)/d,
					    (_00*_22-_02*_20)/d,
					   -(_00*_12-_02*_10)/d,
					    (_10*_21-_11*_20)/d,
					   -(_00*_21-_01*_20)/d,
				   	    (_00*_11-_01*_10)/d );	
}

MtMatrix3& MtMatrix3::operator*=(BtFloat s)
{
	_00 *= s;
	_01 *= s;
	_02 *= s;
	_10 *= s;
	_11 *= s;
	_12 *= s;
	_20 *= s;
	_21 *= s;
	_22 *= s;
	return *this;
}

MtMatrix3& MtMatrix3::operator/=(BtFloat s)
{
	_00 /= s;
	_01 /= s;
	_02 /= s;
	_10 /= s;
	_11 /= s;
	_12 /= s;
	_20 /= s;
	_21 /= s;
	_22 /= s;
	return *this;
}

MtMatrix3 operator+(MtMatrix3 &m1, MtMatrix3 &m2)
{
	return MtMatrix3(	m1._00+m2._00,
						m1._01+m2._01,
						m1._02+m2._02,
						m1._10+m2._10,
						m1._11+m2._11,
						m1._12+m2._12,
						m1._20+m2._20,
						m1._21+m2._21,
						m1._22+m2._22);
}

MtMatrix3 operator-(MtMatrix3 &m1, MtMatrix3 &m2)
{
	return MtMatrix3(	m1._00-m2._00,
						m1._01-m2._01,
						m1._02-m2._02,
						m1._10-m2._10,
						m1._11-m2._11,
						m1._12-m2._12,
						m1._20-m2._20,
						m1._21-m2._21,
						m1._22-m2._22);
}

MtMatrix3 operator/(MtMatrix3 &m, BtFloat s)
{	
	return	MtMatrix3(	m._00/s,
						m._01/s,
						m._02/s,
						m._10/s,
						m._11/s,
						m._12/s,
						m._20/s,
						m._21/s,
						m._22/s);
}

MtMatrix3 MtMatrix3::operator * ( const MtMatrix3& m2) const
{
	return MtMatrix3(	_00*m2._00 + _01*m2._10 + _02*m2._20 ,
						_00*m2._01 + _01*m2._11 + _02*m2._21 ,
						_00*m2._02 + _01*m2._12 + _02*m2._22 ,

						_10*m2._00 + _11*m2._10 + _12*m2._20 ,
						_10*m2._01 + _11*m2._11 + _12*m2._21 ,
						_10*m2._02 + _11*m2._12 + _12*m2._22 ,

						_20*m2._00 + _21*m2._10 + _22*m2._20 ,
						_20*m2._01 + _21*m2._11 + _22*m2._21 ,
						_20*m2._02 + _21*m2._12 + _22*m2._22 );
}

MtMatrix3 operator*(BtFloat s, MtMatrix3 &m)
{
	return MtMatrix3(	m._00*s,
						m._01*s,
						m._02*s,
						m._10*s,
						m._11*s,
						m._12*s,
						m._20*s,
						m._21*s,
						m._22*s);
}

MtMatrix3 operator*(MtMatrix3 &m, BtFloat s)
{
	return MtMatrix3(	m._00*s,
						m._01*s,
						m._02*s,
						m._10*s,
						m._11*s,
						m._12*s,
						m._20*s,
						m._21*s,
						m._22*s);
}

// Public functions ///////////////////////////////////////////////////////////
MtMatrix3 Quaternion2Matrix(MtQuaternion &q)
{
	return MtMatrix3(q);
}

MtVector3 MtMatrix3::Row0() const
{
	return MtVector3( _00, _01, _02 );
}

MtVector3 MtMatrix3::Row1() const
{
	return MtVector3( _10, _11, _12 );
}

MtVector3 MtMatrix3::Row2() const
{
	return MtVector3( _20, _21, _22 );
}

void MtMatrix3::Row0( MtVector3 v3Row )
{
	_00 = v3Row.x;
	_01 = v3Row.y;
	_02 = v3Row.z;
}

void MtMatrix3::Row1( MtVector3 v3Row )
{
	_10 = v3Row.x;
	_11 = v3Row.y;
	_12 = v3Row.z;
}

void MtMatrix3::Row2( MtVector3 v3Row )
{
	_20 = v3Row.x;
	_21 = v3Row.y;
	_22 = v3Row.z;
}

MtVector3 MtMatrix3::Col0() const
{
	return MtVector3( _00, _10, _20 );
}

MtVector3 MtMatrix3::Col1() const
{
	return MtVector3( _01, _11, _21 );
}

MtVector3 MtMatrix3::Col2() const
{
	return MtVector3( _02, _12, _22 );
}

MtVector3 MtMatrix3::Col( BtU32 iIndex ) const
{
	if( iIndex == 0 )
		return MtVector3( _00, _10, _20 );
	else if( iIndex == 1 )
		return MtVector3( _01, _11, _21 );
	else
		return MtVector3( _02, _12, _22 );
}

void MtMatrix3::Col0( MtVector3 v3Col )
{
	_00 = v3Col.x;
	_10 = v3Col.y;
	_20 = v3Col.z;
}

void MtMatrix3::Col1( MtVector3 v3Col )
{
	_01 = v3Col.x;
	_11 = v3Col.y;
	_21 = v3Col.z;
}

void MtMatrix3::Col2( MtVector3 v3Col )
{
	_02 = v3Col.x;
	_12 = v3Col.y;
	_22 = v3Col.z;
}
