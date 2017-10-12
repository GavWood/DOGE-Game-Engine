////////////////////////////////////////////////////////////////////////////////
// MtVector4.h

// Include guard
#ifndef __MtVector4_h__
#define __MtVector4_h__

#include "BtTypes.h"

// Forward prototypes
class MtQuaternion;
class MtMatrix3;
class MtMatrix4;
struct MtVector3;

// Class definition
class MtVector4
{
public:

	// Construction
	MtVector4() {};
	MtVector4( const BtFloat f ) { x = f; y = f; z = f; w = f; }
	MtVector4( const BtFloat* BtFloats);
	MtVector4( const MtVector4& myvector);
	MtVector4( const MtVector3& myvector );
    MtVector4( const MtVector3& myvector, BtFloat w );
	MtVector4( BtFloat nx, BtFloat ny, BtFloat nz, BtFloat nw );

	// Casting
	operator BtFloat* ();
	operator const BtFloat* () const;
	operator MtVector3();

	// Maths operators
	MtVector4 operator * ( const MtVector4& );
	MtVector4 operator * ( const MtMatrix4& matrix ) const;
	MtVector4 operator + ( const MtVector4& ) const;
	MtVector4 operator - ( const MtVector4& ) const;
	MtVector4 operator * ( BtFloat ) const;
	MtVector4 operator - () const;
	MtVector4 operator *= ( BtFloat scale );

	BtFloat DotProduct( const MtVector4& v4 ) const;

	BtFloat x, y, z, w;
};

inline BtFloat MtVector4::DotProduct( const MtVector4& v2 ) const
{
	return x*v2.x + y*v2.y + z*v2.z + w*v2.w;
}

#endif // __MtVector4_H
