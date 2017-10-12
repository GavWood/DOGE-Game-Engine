////////////////////////////////////////////////////////////////////////////////
// MtQuaternion.h

// Include guard
#pragma  once
#include "BtTypes.h"

// Forward prototypes
class MtMatrix3;
class MtMatrix4;
struct MtVector3;

// Class definition
class MtQuaternion
{
public:

	// Member variables
	union
	{
		struct
		{
			BtFloat x, y, z, w;
		};
		BtFloat q[4];
	};

	// Constructors

	MtQuaternion();											// Empty constructor

	MtQuaternion( const MtQuaternion &q );					// Construct from 3x3 matrix

	MtQuaternion( const MtMatrix3 &mat );					// Construct from 3x3 matrix

	MtQuaternion( const MtMatrix4 &mat );					// Construct from 3x3 matrix

	MtQuaternion operator - () const;

	MtQuaternion( BtFloat qx, BtFloat qy, BtFloat qz, BtFloat qw )	// Construct from floats
	{
		x = qx;
		y = qy;
		z = qz;
		w = qw;
	}

	BtFloat DotProduct( const MtQuaternion& v2 ) const;

	// Casting functions //////////////////////////////////////////////////
	operator MtMatrix3();
	operator BtFloat* ();
	operator const BtFloat* () const;

	// Comparison functions ///////////////////////////////////////////////
	BtS32 operator==(const MtQuaternion &op2) const;
	
	// Maths operations ///////////////////////////////////////////////////
	MtQuaternion &operator*=(const BtFloat &op2);
	MtQuaternion &operator*=(const MtQuaternion &op2);
	MtQuaternion &operator+=(const MtQuaternion &op2);

	MtQuaternion operator+(const MtQuaternion &q1);

	// Public functions ///////////////////////////////////////////////////
	void SetIdentity();

	static MtQuaternion SLerp( BtFloat t, MtQuaternion& From, MtQuaternion& To );

	MtQuaternion &Normalise();
};

// Maths operations ///////////////////////////////////////////////////////////
MtQuaternion operator*(MtQuaternion &q, BtFloat s);
MtQuaternion operator*(BtFloat s, MtQuaternion &q);
MtQuaternion operator*(MtQuaternion &q1, MtQuaternion &q2);
MtQuaternion operator*(MtQuaternion &q, MtVector3 &v);
MtQuaternion operator*(MtVector3 &v, MtQuaternion &q);
