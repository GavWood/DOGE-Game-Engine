////////////////////////////////////////////////////////////////////////////////
// MtVector2.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtMath.h"

// Forward declarations
class MtMatrix4;
class MtMatrix3;

// Class definition
struct MtVector2
{
	// Construction
	MtVector2() {};
	MtVector2( const BtFloat f ) { x = f; y = f; }
	MtVector2( const BtFloat * BtFloats);
	MtVector2( const MtVector2& myvector);
	MtVector2( BtFloat nx, BtFloat ny );

	// Casting
	operator BtFloat* ();
	operator const BtFloat* () const;

	// Maths operators
	MtVector2& operator += ( const MtVector2& );
	MtVector2& operator -= ( const MtVector2& );
	MtVector2& operator *= ( BtFloat );
	MtVector2& operator /= ( BtFloat );
	MtVector2  operator - () const;
	MtVector2  operator *  ( BtFloat ) const;
	MtVector2  operator /  ( BtFloat ) const;
	MtVector2  operator *  ( const MtMatrix4& ) const;
	MtVector2  operator *  ( const MtMatrix3& ) const;
	MtVector2  operator *  ( const MtVector2& ) const;
	MtVector2  operator +  ( const MtVector2& ) const;
	MtVector2  operator -  ( const MtVector2& ) const;

	BtFloat						DotProduct( const MtVector2& v2 ) const;

	// Static functions
	static BtFloat				Dot(const MtVector2& v2A, const MtVector2& v2B);
	static BtFloat				DotProduct(const MtVector2& v2A, const MtVector2& v2B);
	BtFloat						CrossProduct( const MtVector2& v2 ) const;
	static MtVector2			Reflect( const MtVector2& v2Velocity, const MtVector2& v2Normal );

	BtFloat						GetLengthSquared();
	BtFloat						GetLength() const;
	MtVector2&					Normalize();
	MtVector2					GetNormalize();
	BtFloat						Magnitude() const;

	// Member variables
	BtFloat						x, y;
};

// Public functions
inline BtFloat MtVector2::GetLengthSquared()
{
	return x*x + y*y;
}

inline BtFloat MtVector2::GetLength() const
{
	return (BtFloat)sqrt( x*x + y*y );
}
