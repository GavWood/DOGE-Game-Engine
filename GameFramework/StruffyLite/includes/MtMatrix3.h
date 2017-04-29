//////////////////////////////////////////////////////////////////////////////
// MtMatrix3

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"

// Forward declarations
class MtQuaternion;
class MtMatrix4;

// Class definition
class MtMatrix3
{
public:
	
	// Member variables
	union
	{
		struct
		{
			BtFloat	_00, _01, _02,
					_10, _11, _12,
					_20, _21, _22;	
		};
		BtFloat m[3][3];
		BtFloat mm[9];
	};

	// Constructors
	MtMatrix3(void);

	MtMatrix3( const MtVector3& row1, const MtVector3& row2, const MtVector3& row3 );

	MtMatrix3(	BtFloat _00, BtFloat _01, BtFloat r1c3, 
				BtFloat r2c1, BtFloat r2c2, BtFloat r2c3, 
				BtFloat r3c1, BtFloat r3c2, BtFloat r3c3 );
	
	MtMatrix3( const MtQuaternion& );

	MtMatrix3( const MtMatrix4& );

	// Static members
	static MtMatrix3				m_identity;
	static MtMatrix3				m_zero;

	// Public functions

	BtFloat*						operator [] (BtU32 i);
	const BtFloat*					operator [] (BtU32 i) const;
	MtMatrix3						operator * (const MtMatrix3& m2) const;

	// Public functions
	
	static MtMatrix3				&GetIdentity() { return m_identity; }
	static MtMatrix3				&GetZero()     { return m_zero; }
	
	MtMatrix3						GetInverse() const;
	MtMatrix3						GetTranspose() const;
	BtFloat							GetDeterminent() const;

	void							SetIdentity();
	
	void							SetRotationX(BtFloat angle);
	void							SetRotationY(BtFloat angle);
	void							SetRotationZ(BtFloat angle);

	void							LookAt( const MtVector3& v3Position, const MtVector3& v3LookAt, const MtVector3& v3Up );

	MtMatrix3&						operator*=(BtFloat s);
	MtMatrix3&						operator/=(BtFloat s);

	// Accessors

	MtVector3 						XAxis() const { return MtVector3(_00, _01, _02); }
	MtVector3 						YAxis() const { return MtVector3(_10, _11, _12); }
	MtVector3 						ZAxis() const { return MtVector3(_20, _21, _22); }

	MtVector3						Row0() const;
	MtVector3						Row1() const;
	MtVector3						Row2() const;

	void							Row0( MtVector3 v4Row );
	void							Row1( MtVector3 v4Row );
	void							Row2( MtVector3 v4Row );

	MtVector3						Col0() const;
	MtVector3						Col1() const;
	MtVector3						Col2() const;
	MtVector3						Col( BtU32 iIndex ) const;

	void							Col0( MtVector3 v4Col );
	void							Col1( MtVector3 v4Col );
	void							Col2( MtVector3 v4Col );

	BtFloat&						operator () ( BtU32 Row, BtU32 Col ) { return m[Row][Col]; }
	BtFloat							operator () ( BtU32 Row, BtU32 Col ) const { return m[Row][Col]; }
};

MtMatrix3 operator+(MtMatrix3 &m1, MtMatrix3 &m2);
MtMatrix3 operator-(MtMatrix3 &m1, MtMatrix3 &m2);
MtMatrix3 operator/(MtMatrix3 &m, BtFloat s);
MtMatrix3 operator*(MtMatrix3 &m, BtFloat s);
MtMatrix3 operator*(BtFloat s, MtMatrix3 &m);
