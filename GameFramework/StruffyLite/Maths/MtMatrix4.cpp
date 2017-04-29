// Local includes
#include <memory.h>
#include "BtMemory.h"
#include "MtMath.h"

// Static members
MtMatrix4 MtMatrix4::m_identity(1.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 1.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f
							 );

// Constructors
MtMatrix4::MtMatrix4( const BtFloat *mat )
{
	memcpy( &_00, mat, sizeof(BtFloat) * 16 );
}

MtMatrix4::MtMatrix4(BtFloat _00, BtFloat _01, BtFloat _02, BtFloat _03,
		 		   BtFloat _10, BtFloat _11, BtFloat _12, BtFloat _13,
				   BtFloat _20, BtFloat _21, BtFloat _22, BtFloat _23,
				   BtFloat _30, BtFloat _31, BtFloat _32, BtFloat _33 )
{
	this->_00 = _00;
	this->_01 = _01;
	this->_02 = _02;
	this->_03 = _03;

	this->_10 = _10;
	this->_11 = _11;
	this->_12 = _12;
	this->_13 = _13;

	this->_20 = _20;
	this->_21 = _21;
	this->_22 = _22;
	this->_23 = _23;

	this->_30 = _30;
	this->_31 = _31;
	this->_32 = _32;
	this->_33 = _33;
}

MtMatrix4::MtMatrix4( const MtMatrix4 &mat )
{
	memcpy( &_00, &mat, sizeof(BtFloat) * 16 );
}

BtFloat* MtMatrix4::operator [] (BtU32 i)
{
	return m[i];
}

const BtFloat* MtMatrix4::operator [] (BtU32 i) const
{
	return m[i];
}

MtMatrix4::MtMatrix4( const MtQuaternion& q )
{
	// Set the identity matrix
	SetIdentity();

	// Create rotation matrix
	MtMatrix3 m3Rotation = MtMatrix3( q );

	// Row 1
	_00 = m3Rotation._00;
	_01 = m3Rotation._01;
	_02 = m3Rotation._02;

	// Row 2
	_10 = m3Rotation._10;
	_11 = m3Rotation._11;
	_12 = m3Rotation._12;

	// Row 3
	_20 = m3Rotation._20;
	_21 = m3Rotation._21;
	_22 = m3Rotation._22;
}

MtMatrix4::MtMatrix4( const MtMatrix3 &m3Matrix)
{
	// Set the identity matrix
	SetIdentity();

	// Row 1
	_00 = m3Matrix._00;
	_01 = m3Matrix._01;
	_02 = m3Matrix._02;

	// Row 2
	_10 = m3Matrix._10;
	_11 = m3Matrix._11;
	_12 = m3Matrix._12;

	// Row 3
	_20 = m3Matrix._20;
	_21 = m3Matrix._21;
	_22 = m3Matrix._22;
}

// Assignment operators ///////////////////////////////////////////////////////
MtMatrix4& MtMatrix4::operator = ( const MtQuaternion& q )
{
	BtFloat xx2 = (q.x + q.x ) * q.x;
	BtFloat yy2 = (q.y + q.y ) * q.y;
	BtFloat zz2 = (q.z + q.z ) * q.z;

	BtFloat xy = 2.0f * q.x * q.y;
	BtFloat yz = 2.0f * q.y * q.z;
	BtFloat xz = 2.0f * q.x * q.z;
	BtFloat wx = 2.0f * q.w * q.x;
	BtFloat wy = 2.0f * q.w * q.y;
	BtFloat wz = 2.0f * q.w * q.z;

	// Set the identity matrix
	SetIdentity();

	// Calculate new matrix entries:
	_00 = 1.0f - yy2-zz2;		_01 = xy+wz;				_02 = xz-wy;
	_10 = xy-wz;				_11 = 1.0f - xx2-zz2;		_12 = yz+wx;
	_20 = xz+wy;				_21 = yz-wx;				_22 = 1.0f - xx2-yy2;
	
	return *this;
}

// Maths operations ///////////////////////////////////////////////////////////

// Public functions ///////////////////////////////////////////////////////////
void MtMatrix4::SetScale(BtFloat x, BtFloat y, BtFloat z )
{
	SetIdentity();

	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
}

void MtMatrix4::SetScale( BtFloat scale )
{
	SetIdentity();

	m[0][0] = scale;
	m[1][1] = scale;
	m[2][2] = scale;
}

void MtMatrix4::SetScale( const MtVector3 &scale )
{
	SetIdentity();

	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void MtMatrix4::SetReflection( MtPlane &plane )
{
	MtMatrix4 matrix;

	BtFloat x = plane.m_v3Normal.x;
	BtFloat y = plane.m_v3Normal.y;
	BtFloat z = plane.m_v3Normal.z;
	BtFloat d = plane.m_fDistance;

	*this = MtMatrix4(	-2 * x * x + 1,  -2 * y * x,		-2 * z * x,			0,
						-2 * x * y,      -2 * y * y + 1,	-2 * z * y,			0,
						-2 * x * z,		 -2 * y * z,		-2 * z * z + 1,		0,
						-2 * x * d,		 -2 * y * d,		-2 * z * d,			1 );
}

void MtMatrix4::LookAt( const MtVector3& v3Position, const MtVector3& v3LookAt, const MtVector3& v3Up )
{
	MtVector3 v3Z = v3LookAt - v3Position;
	v3Z.Normalize();

	MtVector3 v3X = MtVector3::CrossProduct( v3Up, v3Z );
	v3X.Normalize();

	MtVector3 v3Y = MtVector3::CrossProduct( v3Z, v3X );
	v3Y.Normalize();

	Row0( MtVector4( v3X.X(),  v3Y.X(),  v3Z.X(),  0.0f ) );
	Row1( MtVector4( v3X.Y(),  v3Y.Y(),  v3Z.Y(),  0.0f ) );
	Row2( MtVector4( v3X.Z(),  v3Y.Z(),  v3Z.Z(),  0.0f ) );
	Row3( MtVector4( 0, 0, 0, 1.0f ) );
}

///////////////////////////////////////////////////////////////////////////////
// Build an orthographic projection matrix

void MtMatrix4::BuildOrthographicProjectionMatrix( BtFloat w, BtFloat h, BtFloat n, BtFloat f )
{
	// Fill in entries
	m[0][0] =  2/w; m[0][1] = 0;   m[0][2] = 0;		   m[0][3] = 0;
	m[1][0] =  0;   m[1][1] = 2/h; m[1][2] = 0;		   m[1][3] = 0;
	m[2][0] =  0;   m[2][1] = 0;   m[2][2] = 1/(f-n);  m[2][3] = 0;
	m[3][0] =  0;   m[3][1] = 0;   m[3][2] = n/(n-f);  m[3][3] = 1;
}

///////////////////////////////////////////////////////////////////////////////
// BuildLeftHandedProjectionMatrix

void MtMatrix4::BuildLeftHandedProjectionMatrix(BtFloat znear, BtFloat zfar, BtFloat aspect, BtFloat field_of_view )
{
	float xymax = znear * MtTan( field_of_view * 0.5f );
	float ymin = -xymax;
	float xmin = -xymax;

	float width = xymax - xmin;
	float height = xymax - ymin;

	float depth = zfar - znear;
	float q  = ( zfar + znear ) / depth;
	float qn = -2 * (zfar * znear) / depth;

	float w = 2 * znear / width;
	w = w / aspect;
	float h = 2 * znear / height;

	m[0][0] = w;		m[0][1] = 0;		m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;		m[1][1] = h;		m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;		m[2][1] = 0;		m[2][2] = q;    m[2][3] = 1;
	m[3][0] = 0;		m[3][1] = 0;		m[3][2] = qn;	m[3][3] = 0;
}

///////////////////////////////////////////////////////////////////////////////
// BuildRightHandedProjectionMatrix

void MtMatrix4::BuildRightHandedProjectionMatrix(BtFloat znear, BtFloat zfar, BtFloat aspect, BtFloat field_of_view )
{
	float xymax = znear * MtTan( field_of_view * 0.5f );
	float ymin = -xymax;
	float xmin = -xymax;

	float width = xymax - xmin;
	float height = xymax - ymin;

	float depth = zfar - znear;
	float q  = -( zfar + znear ) / depth;
	float qn = -2 * (zfar * znear) / depth;

	float w = 2 * znear / width;
	w = w / aspect;
	float h = 2 * znear / height;

	m[0][0] = w;		m[0][1] = 0;		m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;		m[1][1] = h;		m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;		m[2][1] = 0;		m[2][2] = q;    m[2][3] = -1;
	m[3][0] = 0;		m[3][1] = 0;		m[3][2] = qn;	m[3][3] = 0;
}

MtMatrix3 MtMatrix4::GetRotation() const
{
	return MtMatrix3( _00, _01, _02,
			 		  _10, _11, _12,
					  _20, _21, _22 );
}

MtVector3 MtMatrix4::GetTranslation() const
{
	return MtVector3( _30, _31, _32 );
}

void MtMatrix4::SetTranslation(MtVector3 translation)
{
	SetIdentity();

	_30 = translation.x;
	_31 = translation.y;
	_32 = translation.z;
}

void MtMatrix4::SetRotation( const MtMatrix3 &rotation )
{
	SetIdentity();

	_00 = rotation._00; _01 = rotation._01; _02 = rotation._02;
	_10 = rotation._10; _11 = rotation._11; _12 = rotation._12;
	_20 = rotation._20; _21 = rotation._21; _22 = rotation._22;
}

void MtMatrix4::SetRotation(const MtVector3 & u, BtFloat radians)
{
	BtFloat c = MtCos(radians);
	BtFloat l_c = 1 - c;

	BtFloat s = MtSin(radians);

	*this = MtMatrix4(
		u.x*u.x + (1 - u.x*u.x)*c, u.x*u.y*l_c + u.z*s, u.x*u.z*l_c - u.y*s, 0,
		u.x*u.y*l_c - u.z*s, u.y*u.y + (1 - u.y*u.y)*c, u.y*u.z*l_c + u.x*s, 0,
		u.x*u.z*l_c + u.y*s, u.y*u.z*l_c - u.x*s, u.z*u.z + (1 - u.z*u.z)*c, 0,
		0, 0, 0, 1
		);
}

void MtMatrix4::SetQuaternion( MtQuaternion& q )
{
	// Set the identity matrix
	SetIdentity();

	// Create rotation matrix
	MtMatrix3 m3Rotation = MtMatrix3( q );

	// Row 1
	_00 = m3Rotation._00;
	_01 = m3Rotation._01;
	_02 = m3Rotation._02;

	// Row 2
	_10 = m3Rotation._10;
	_11 = m3Rotation._11;
	_12 = m3Rotation._12;

	// Row 3
	_20 = m3Rotation._20;
	_21 = m3Rotation._21;
	_22 = m3Rotation._22;
}


void MtMatrix4::SetIdentity()
{
	_00 = 1.0f; _01 =   0;  _02 =    0; _03 = 0;
	_10 =    0; _11 = 1.0f; _12 =    0; _13 = 0;
	_20 =    0; _21 =    0; _22 = 1.0f; _23 = 0;
	_30 =    0; _31 =    0; _32 =    0; _33 = 1.0f;
}

void MtMatrix4::SetTranslation(BtFloat x, BtFloat y, BtFloat z)
{
	SetIdentity();

	_30 = x;
	_31 = y;
	_32 = z;
}

// _00    _01   _02
// _10    _11   _12
// _20    _21   _22

void MtMatrix4::SetRotationX(BtFloat theta)
{
	BtFloat cos_theta = (BtFloat) cos(theta);
	BtFloat sin_theta = (BtFloat) sin(theta);

	SetIdentity();

	_11 =  cos_theta; _12 = sin_theta;
	_21 = -sin_theta; _22 = cos_theta;
}

void MtMatrix4::SetRotationY(BtFloat theta)
{
	BtFloat cos_theta = (BtFloat) cos(theta);
	BtFloat sin_theta = (BtFloat) sin(theta);

	SetIdentity();

	_00 =  cos_theta; _02 = sin_theta;
	_20 = -sin_theta; _22 = cos_theta;
}

void MtMatrix4::SetRotationZ(BtFloat theta)
{
	BtFloat cos_theta = (BtFloat) cos(theta);
	BtFloat sin_theta = (BtFloat) sin(theta);

	SetIdentity();

	_00 =  cos_theta; _01 = sin_theta;
	_10 = -sin_theta; _11 = cos_theta;
}

MtMatrix4 MtMatrix4::GetInverse() const
{
	BtFloat inv[16];
	BtFloat det;
	BtU32 i;

	inv[0] = mm[5] * mm[10] * mm[15] -
			 mm[5] * mm[11] * mm[14] -
			 mm[9] * mm[6] * mm[15] +
			 mm[9] * mm[7] * mm[14] +
			 mm[13] * mm[6] * mm[11] -
			 mm[13] * mm[7] * mm[10];

	inv[4] = -mm[4] * mm[10] * mm[15] +
			  mm[4] * mm[11] * mm[14] +
			  mm[8] * mm[6] * mm[15] -
			  mm[8] * mm[7] * mm[14] -
			  mm[12] * mm[6] * mm[11] +
			  mm[12] * mm[7] * mm[10];

	inv[8] = mm[4] * mm[9] * mm[15] -
			 mm[4] * mm[11] * mm[13] -
			 mm[8] * mm[5] * mm[15] +
			 mm[8] * mm[7] * mm[13] +
			 mm[12] * mm[5] * mm[11] -
			 mm[12] * mm[7] * mm[9];

	inv[12] = -mm[4] * mm[9] * mm[14] +
			   mm[4] * mm[10] * mm[13] +
			   mm[8] * mm[5] * mm[14] -
			   mm[8] * mm[6] * mm[13] -
			   mm[12] * mm[5] * mm[10] +
			   mm[12] * mm[6] * mm[9];

	inv[1] = -mm[1] * mm[10] * mm[15] +
			  mm[1] * mm[11] * mm[14] +
			  mm[9] * mm[2] * mm[15] -
			  mm[9] * mm[3] * mm[14] -
			  mm[13] * mm[2] * mm[11] +
			  mm[13] * mm[3] * mm[10];

	inv[5] = mm[0] * mm[10] * mm[15] -
			 mm[0] * mm[11] * mm[14] -
			 mm[8] * mm[2] * mm[15] +
			 mm[8] * mm[3] * mm[14] +
			 mm[12] * mm[2] * mm[11] -
			 mm[12] * mm[3] * mm[10];

	inv[9] = -mm[0] * mm[9] * mm[15] +
			  mm[0] * mm[11] * mm[13] +
			  mm[8] * mm[1] * mm[15] -
			  mm[8] * mm[3] * mm[13] -
			  mm[12] * mm[1] * mm[11] +
			  mm[12] * mm[3] * mm[9];

	inv[13] = mm[0] * mm[9] * mm[14] -
			  mm[0] * mm[10] * mm[13] -
			  mm[8] * mm[1] * mm[14] +
			  mm[8] * mm[2] * mm[13] +
			  mm[12] * mm[1] * mm[10] -
			  mm[12] * mm[2] * mm[9];

	inv[2] = mm[1] * mm[6] * mm[15] -
			 mm[1] * mm[7] * mm[14] -
			 mm[5] * mm[2] * mm[15] +
			 mm[5] * mm[3] * mm[14] +
			 mm[13] * mm[2] * mm[7] -
			 mm[13] * mm[3] * mm[6];

	inv[6] = -mm[0] * mm[6] * mm[15] +
			  mm[0] * mm[7] * mm[14] +
			  mm[4] * mm[2] * mm[15] -
			  mm[4] * mm[3] * mm[14] -
			  mm[12] * mm[2] * mm[7] +
			  mm[12] * mm[3] * mm[6];

	inv[10] = mm[0] * mm[5] * mm[15] -
			  mm[0] * mm[7] * mm[13] -
			  mm[4] * mm[1] * mm[15] +
			  mm[4] * mm[3] * mm[13] +
			  mm[12] * mm[1] * mm[7] -
			  mm[12] * mm[3] * mm[5];

	inv[14] = -mm[0] * mm[5] * mm[14] +
			   mm[0] * mm[6] * mm[13] +
			   mm[4] * mm[1] * mm[14] -
			   mm[4] * mm[2] * mm[13] -
			   mm[12] * mm[1] * mm[6] +
			   mm[12] * mm[2] * mm[5];

	inv[3] = -mm[1] * mm[6] * mm[11] +
			  mm[1] * mm[7] * mm[10] +
			  mm[5] * mm[2] * mm[11] -
			  mm[5] * mm[3] * mm[10] -
			  mm[9] * mm[2] * mm[7] +
			  mm[9] * mm[3] * mm[6];

	inv[7] = mm[0] * mm[6] * mm[11] -
			 mm[0] * mm[7] * mm[10] -
			 mm[4] * mm[2] * mm[11] +
			 mm[4] * mm[3] * mm[10] +
			 mm[8] * mm[2] * mm[7] -
			 mm[8] * mm[3] * mm[6];

	inv[11] = -mm[0] * mm[5] * mm[11] +
			   mm[0] * mm[7] * mm[9] +
			   mm[4] * mm[1] * mm[11] -
			   mm[4] * mm[3] * mm[9] -
			   mm[8] * mm[1] * mm[7] +
			   mm[8] * mm[3] * mm[5];

	inv[15] = mm[0] * mm[5] * mm[10] -
			  mm[0] * mm[6] * mm[9] -
			  mm[4] * mm[1] * mm[10] +
			  mm[4] * mm[2] * mm[9] +
			  mm[8] * mm[1] * mm[6] -
			  mm[8] * mm[2] * mm[5];

	det = mm[0] * inv[0] + mm[1] * inv[4] + mm[2] * inv[8] + mm[3] * inv[12];

	if(det == 0)
		return MtMatrix4::m_identity;

	det = 1.0f / det;

	MtMatrix4 m4Return;

	for(i = 0; i < 16; i++)
	{
		m4Return.mm[i] = inv[i] * det;
	}
	return m4Return;
}

MtMatrix4 MtMatrix4::GetTranspose() const
{
	MtMatrix4 retMatrix;

	retMatrix(0,0) = m[0][0];	retMatrix(0,1) = m[1][0];	retMatrix(0,2) = m[2][0];	retMatrix(0,3) = m[3][0];
	retMatrix(1,0) = m[0][1];	retMatrix(1,1) = m[1][1];	retMatrix(1,2) = m[2][1];	retMatrix(1,3) = m[3][1];
	retMatrix(2,0) = m[0][2];	retMatrix(2,1) = m[1][2];	retMatrix(2,2) = m[2][2];	retMatrix(2,3) = m[3][2];
	retMatrix(3,0) = m[0][3];	retMatrix(3,1) = m[1][3];	retMatrix(3,2) = m[2][3];	retMatrix(3,3) = m[3][3];

	return retMatrix;
}

MtMatrix4& MtMatrix4::operator+=(const MtMatrix4 &m)
{
	_00 += m._00;
	_01 += m._01;
	_02 += m._02;
	_03 += m._03;
	_10 += m._10;
	_11 += m._11;
	_12 += m._12;
	_13 += m._13;
	_20 += m._20;
	_21 += m._21;
	_22 += m._22;
	_23 += m._23;
	_30 += m._30;
	_31 += m._31;
	_32 += m._32;
	_33 += m._33;
	return *this;
}

MtMatrix4& MtMatrix4::operator-=(const MtMatrix4 &m)
{
	_00 -= m._00;
	_01 -= m._01;
	_02 -= m._02;
	_03 -= m._03;
	_10 -= m._10;
	_11 -= m._11;
	_12 -= m._12;
	_13 -= m._13;
	_20 -= m._20;
	_21 -= m._21;
	_22 -= m._22;
	_23 -= m._23;
	_30 -= m._30;
	_31 -= m._31;
	_32 -= m._32;
	_33 -= m._33;
	return *this;
}

MtMatrix4& MtMatrix4::operator=(const MtMatrix4 &m)
{
	_00 = m._00;
	_01 = m._01;
	_02 = m._02;
	_03 = m._03;
	_10 = m._10;
	_11 = m._11;
	_12 = m._12;
	_13 = m._13;
	_20 = m._20;
	_21 = m._21;
	_22 = m._22;
	_23 = m._23;
	_30 = m._30;
	_31 = m._31;
	_32 = m._32;
	_33 = m._33;
	return *this;
}

MtMatrix4& MtMatrix4::operator*=(BtFloat s)
{
	_00 *= s;
	_01 *= s;
	_02 *= s;
	_03 *= s;
	_10 *= s;
	_11 *= s;
	_12 *= s;
	_13 *= s;
	_20 *= s;
	_21 *= s;
	_22 *= s;
	_23 *= s;
	_30 *= s;
	_31 *= s;
	_32 *= s;
	_33 *= s;
	return *this;
}

MtMatrix4& MtMatrix4::operator/=(BtFloat s)
{
	_00 /= s;
	_01 /= s;
	_02 /= s;
	_03 /= s;
	_10 /= s;
	_11 /= s;
	_12 /= s;
	_13 /= s;
	_20 /= s;
	_21 /= s;
	_22 /= s;
	_23 /= s;
	_30 /= s;
	_31 /= s;
	_32 /= s;
	_33 /= s;
	return *this;
}

MtMatrix4 MtMatrix4::operator * ( const MtMatrix4& m2) const
{
	return MtMatrix4(	_00*m2._00 + _01*m2._10 + _02*m2._20 + _03*m2._30,
						_00*m2._01 + _01*m2._11 + _02*m2._21 + _03*m2._31,
						_00*m2._02 + _01*m2._12 + _02*m2._22 + _03*m2._32,
						_00*m2._03 + _01*m2._13 + _02*m2._23 + _03*m2._33,

						_10*m2._00 + _11*m2._10 + _12*m2._20 + _13*m2._30,
						_10*m2._01 + _11*m2._11 + _12*m2._21 + _13*m2._31,
						_10*m2._02 + _11*m2._12 + _12*m2._22 + _13*m2._32,
						_10*m2._03 + _11*m2._13 + _12*m2._23 + _13*m2._33,

						_20*m2._00 + _21*m2._10 + _22*m2._20 + _23*m2._30,
						_20*m2._01 + _21*m2._11 + _22*m2._21 + _23*m2._31,
						_20*m2._02 + _21*m2._12 + _22*m2._22 + _23*m2._32,
						_20*m2._03 + _21*m2._13 + _22*m2._23 + _23*m2._33,

						_30*m2._00 + _31*m2._10 + _32*m2._20 + _33*m2._30,
						_30*m2._01 + _31*m2._11 + _32*m2._21 + _33*m2._31,
						_30*m2._02 + _31*m2._12 + _32*m2._22 + _33*m2._32,
						_30*m2._03 + _31*m2._13 + _32*m2._23 + _33*m2._33
					);
}

MtMatrix4& MtMatrix4::operator*=(const MtMatrix4& m2)
{
	MtMatrix4 temp = MtMatrix4(	_00*m2._00 + _01*m2._10 + _02*m2._20 + _03*m2._30,
								_00*m2._01 + _01*m2._11 + _02*m2._21 + _03*m2._31,
								_00*m2._02 + _01*m2._12 + _02*m2._22 + _03*m2._32,
								_00*m2._03 + _01*m2._13 + _02*m2._23 + _03*m2._33,

								_10*m2._00 + _11*m2._10 + _12*m2._20 + _13*m2._30,
								_10*m2._01 + _11*m2._11 + _12*m2._21 + _13*m2._31,
								_10*m2._02 + _11*m2._12 + _12*m2._22 + _13*m2._32,
								_10*m2._03 + _11*m2._13 + _12*m2._23 + _13*m2._33,

								_20*m2._00 + _21*m2._10 + _22*m2._20 + _23*m2._30,
								_20*m2._01 + _21*m2._11 + _22*m2._21 + _23*m2._31,
								_20*m2._02 + _21*m2._12 + _22*m2._22 + _23*m2._32,
								_20*m2._03 + _21*m2._13 + _22*m2._23 + _23*m2._33,

								_30*m2._00 + _31*m2._10 + _32*m2._20 + _33*m2._30,
								_30*m2._01 + _31*m2._11 + _32*m2._21 + _33*m2._31,
								_30*m2._02 + _31*m2._12 + _32*m2._22 + _33*m2._32,
								_30*m2._03 + _31*m2._13 + _32*m2._23 + _33*m2._33
						);
	*this = temp;

	return *this;
}

MtVector4 MtMatrix4::Row0() const
{
	return MtVector4( _00, _01, _02, _03 );
}

MtVector4 MtMatrix4::Row1() const
{
	return MtVector4( _10, _11, _12, _13 );
}

MtVector4 MtMatrix4::Row2() const
{
	return MtVector4( _20, _21, _22, _23 );
}

MtVector4 MtMatrix4::Row3() const
{
	return MtVector4( _30, _31, _32, _33 );
}

void MtMatrix4::Row0( MtVector4 v4Row )
{
	_00 = v4Row.x;
	_01 = v4Row.y;
	_02 = v4Row.z;
	_03 = v4Row.w;
}

void MtMatrix4::Row1( MtVector4 v4Row )
{
	_10 = v4Row.x;
	_11 = v4Row.y;
	_12 = v4Row.z;
	_13 = v4Row.w;
}

void MtMatrix4::Row2( MtVector4 v4Row )
{
	_20 = v4Row.x;
	_21 = v4Row.y;
	_22 = v4Row.z;
	_23 = v4Row.w;
}

void MtMatrix4::Row3( MtVector4 v4Row )
{
	_30 = v4Row.x;
	_31 = v4Row.y;
	_32 = v4Row.z;
	_33 = v4Row.w;
}

MtVector4 MtMatrix4::Col0() const
{
	return MtVector4( _00, _10, _20, _30 );
}

MtVector4 MtMatrix4::Col1() const
{
	return MtVector4( _01, _11, _21, _31 );
}

MtVector4 MtMatrix4::Col2() const
{
	return MtVector4( _02, _12, _22, _32 );
}

MtVector4 MtMatrix4::Col3() const
{
	return MtVector4( _03, _13, _23, _33 );
}

void MtMatrix4::Col0( MtVector4 v4Col )
{
	_00 = v4Col.x;
	_10 = v4Col.y;
	_20 = v4Col.z;
	_30 = v4Col.w;
}

void MtMatrix4::Col1( MtVector4 v4Col )
{
	_01 = v4Col.x;
	_11 = v4Col.y;
	_21 = v4Col.z;
	_31 = v4Col.w;
}

void MtMatrix4::Col2( MtVector4 v4Col )
{
	_02 = v4Col.x;
	_12 = v4Col.y;
	_22 = v4Col.z;
	_32 = v4Col.w;
}

void MtMatrix4::Col3( MtVector4 v4Col )
{
	_03 = v4Col.x;
	_13 = v4Col.y;
	_23 = v4Col.z;
	_33 = v4Col.w;
}
