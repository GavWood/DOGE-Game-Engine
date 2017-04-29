////////////////////////////////////////////////////////////////////////////////
// MtVector3.h

// Includes
#pragma once
#include "BtTypes.h"

// Forward prototypes
class MtQuaternion;
class MtMatrix3;
class MtMatrix4;
class MtVector4;
struct MtVector2;

#define CONST_MTVECTOR3 const MtVector3&

// Class definition
struct MtVector3
{
public:
	BtFloat x, y, z;

	// Construction
	MtVector3() {};
	MtVector3( const BtFloat f ) { x = f; y = f; z = f; }
	MtVector3( const BtFloat * BtFloats);
	MtVector3( const MtVector3& myvector);
	MtVector3( const MtVector2& myvector, BtFloat z);
	MtVector3( BtFloat nx, BtFloat ny, BtFloat nz );

	// Casting
	operator MtVector4();
	operator MtVector2();

	// Operators
	MtVector3& operator += ( const MtVector3& );
	MtVector3& operator -= ( const MtVector3& );
	MtVector3& operator *= ( BtFloat );
	MtVector3& operator *= ( const MtQuaternion& );
	MtVector3& operator *= ( const MtMatrix4& );
	MtVector3& operator *= ( const MtMatrix3& );
	MtVector3& operator /= ( BtFloat );

	MtVector3  operator + () const;
	MtVector3  operator - () const;
	MtVector3  operator + ( const MtVector3& ) const;
	MtVector3  operator - ( const MtVector3& ) const;
	MtVector3  operator ^ ( const MtVector3& ) const;
	MtVector3  operator * ( const MtVector3& ) const;
	MtVector3  operator * ( BtFloat scale ) const;
	MtVector3  operator * ( const MtQuaternion& ) const;
	MtVector3  operator * ( const MtMatrix4& );
	MtVector3  operator * ( const MtMatrix3& );
	MtVector3  operator * ( const MtMatrix4& ) const;
	MtVector3  operator * ( const MtMatrix3& ) const;
	MtVector3  operator / ( BtFloat ) const;

	BtFloat DotProduct( const MtVector3& v3  ) const;
	MtVector3 CrossProduct( const MtVector3& v3 ) const;

	// Static functions
	static MtVector3 Cross(const MtVector3& v3A, const MtVector3& v3B);
	static MtVector3 CrossProduct(const MtVector3& v3A, const MtVector3& v3B);
	static BtFloat   Dot(const MtVector3& v3A, const MtVector3& v3B);
	static BtFloat   DotProduct(const MtVector3& v3A, const MtVector3& v3B);

	MtVector3 GetNormalize() const;
	MtVector3 GetNormalise() const;

	// Comparisons
	BtBool operator == ( const MtVector3& ) const;
	BtBool operator != ( const MtVector3& ) const;

	// Accessors
	const BtFloat X() const { return x; }
	const BtFloat Y() const { return y; }
	const BtFloat Z() const { return z; }

	BtFloat operator[]( BtU32 i) const
	{
		if( i == 0 )
		{
			return x;
		}
		else if( i == 1 )
		{
			return y;
		}
		else
		{
			return z;
		}
	}

	// Accessors
	void X(BtFloat fX) { x = fX; }
	void Y(BtFloat fY) { y = fY; }
	void Z(BtFloat fZ) { z = fZ; }

	// Public functions
	MtVector3 Normalize();
	MtVector3 Normalise();
	MtVector3 Min (const MtVector3 &v1);
	MtVector3 Max (const MtVector3 &v1);
	BtFloat GetLengthSquared();
	BtFloat Magnitude() const;
	BtFloat GetLength() const;
	
	static MtVector3 ZERO;

	// Return the midpoint between two vectors
	static MtVector3 MidPoint(MtVector3 &p1, MtVector3 &p2);
};

MtVector3 operator*(MtMatrix3 &m, MtVector3 &v);
MtVector3 operator*(MtMatrix3 &v, MtVector3 &m);
