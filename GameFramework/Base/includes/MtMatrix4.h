//////////////////////////////////////////////////////////////////////////////
// MtMatrix4

// Include guard
#pragma once
#include <math.h>
#include "BtTypes.h"
#include "MtQuaternion.h"
#include "MtVector3.h"

// Forward declarations
class MtPlane;
struct MtVector3;
class MtVector4;
class MtMatrix3;
class MtMatrix4;

// Class definition
class MtMatrix4
{
public:

	union
	{
		struct
		{
			//				c0   c1   c2   c3
			BtFloat        _00, _01, _02, _03;	// r0
			BtFloat        _10, _11, _12, _13;	// r1
			BtFloat        _20, _21, _22, _23;	// r2
			BtFloat        _30, _31, _32, _33;	// r3
		};
		BtFloat m[4][4];
		BtFloat mm[16];
	};

	MtMatrix4() {};
	MtMatrix4( const BtFloat * );
	MtMatrix4( const MtMatrix4& );
	MtMatrix4( const MtMatrix3& );
	MtMatrix4( const MtQuaternion& );
	MtMatrix4( BtFloat _00, BtFloat _01, BtFloat _02, BtFloat _03,
			   BtFloat _10, BtFloat _11, BtFloat _12, BtFloat _13,
			   BtFloat _20, BtFloat _21, BtFloat _22, BtFloat _23,
			   BtFloat _30, BtFloat _31, BtFloat _32, BtFloat _33 );

	// Casting operators
	
	BtFloat*			operator [] (BtU32 i);
	const BtFloat*	operator [] (BtU32 i) const;

	// Static members
	static MtMatrix4 m_identity;

	// Public functions
	static MtMatrix4 &GetIdentity() { return m_identity; }

	void SetReflection(MtPlane &plane);

	void LookAt( const MtVector3& v3Position, const MtVector3& v3LookAt, const MtVector3& v3Up );

	MtMatrix3 GetRotation() const;
	MtVector3 GetTranslation() const;
	void SetTranslation(MtVector3 translation);

	void SetIdentity();
	void SetTranslation(BtFloat x, BtFloat y, BtFloat z);
	void SetQuaternion( MtQuaternion& q );
	void SetScale( BtFloat x, BtFloat y, BtFloat z );
		
		void							SetScale( const MtVector3 &scale );
		void							SetScale( BtFloat scale );

	void SetRotation(const MtMatrix3 &rot);
	void SetRotationX(BtFloat angle);
	void SetRotationY(BtFloat angle);
	void SetRotationZ(BtFloat angle);
	void SetRotation(const MtVector3 & u, BtFloat radians);

	MtMatrix4 GetInverse() const;
	MtMatrix4 GetTranspose() const;

	// Static public functions
	void BuildPerspProjMat( float fov, float aspect, float znear, float zfar);

	void BuildLeftHandedProjectionMatrix( BtFloat near_plane,
										  BtFloat far_plane,
										  BtFloat aspect_ratio,
										  BtFloat field_of_view = 0.8f );

	void BuildRightHandedProjectionMatrix( BtFloat near_plane,
										   BtFloat far_plane,
										   BtFloat aspect_ratio,
										   BtFloat field_of_view = 0.8f );

	void BuildOrthographicProjectionMatrix(BtFloat w, BtFloat h, BtFloat n, BtFloat f);

	// Accessors
	MtVector4						Row0() const;
	MtVector4						Row1() const;
	MtVector4						Row2() const;
	MtVector4						Row3() const;

	void							Row0( MtVector4 v4Row );
	void							Row1( MtVector4 v4Row );
	void							Row2( MtVector4 v4Row );
	void							Row3( MtVector4 v4Row );

	MtVector3 						XAxis() const { return MtVector3(_00, _01, _02); }
	MtVector3 						YAxis() const { return MtVector3(_10, _11, _12); }
	MtVector3 						ZAxis() const { return MtVector3(_20, _21, _22); }

	MtVector4						Col0() const;
	MtVector4						Col1() const;
	MtVector4						Col2() const;
	MtVector4						Col3() const;

	void							Col0( MtVector4 v4Col );
	void							Col1( MtVector4 v4Col );
	void							Col2( MtVector4 v4Col );
	void							Col3( MtVector4 v4Col );

	BtFloat& operator () ( BtU32 Row, BtU32 Col ) { return m[Row][Col]; }
	BtFloat  operator () ( BtU32 Row, BtU32 Col ) const { return m[Row][Col]; }

	// Assignment operators
	MtMatrix4& operator *= ( const MtMatrix4& );
	MtMatrix4& operator += ( const MtMatrix4& );
	MtMatrix4& operator -= ( const MtMatrix4& );
	MtMatrix4& operator *= ( BtFloat );
	MtMatrix4& operator /= ( BtFloat );
	MtMatrix4& operator =  ( const MtQuaternion& );
	MtMatrix4& operator =  ( const MtMatrix4& );

	// Unary operators
	MtMatrix4 operator + () const;
	MtMatrix4 operator - () const;

	// Operators
	MtMatrix4 operator * ( const MtMatrix4& ) const;
	MtMatrix4 operator + ( const MtMatrix4& ) const;
	MtMatrix4 operator - ( const MtMatrix4& ) const;
	MtMatrix4 operator * ( BtFloat ) const;
	MtMatrix4 operator / ( BtFloat ) const;

	BtBool operator == ( const MtMatrix4& ) const;
	BtBool operator != ( const MtMatrix4& ) const;
};

MtMatrix4 operator * ( BtFloat, const MtMatrix4& );
