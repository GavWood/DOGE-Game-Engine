////////////////////////////////////////////////////////////////////////////////
// MtTriangle.h

// Include guard
#pragma once

// Forward prototypes
class MtPlane;
class MtMatrix4;
struct MtVector3;
class MtVector4;

// Class definition
class MtTriangle
{
public:
    // Constructors
	MtTriangle() {};

	MtTriangle( const MtVector3 &v1, const MtVector3 &v2, const MtVector3 &v3 )
	{
		a = v1;
		b = v2;
		c = v3;

		FindNormal();
	}

	MtTriangle( const MtVector4 &v1, const MtVector4 &v2, const MtVector4 &v3 )
	{
		a = MtVector3(v1.x, v1.y, v1.z);
		b = MtVector3(v2.x, v2.y, v2.z);
		c = MtVector3(v3.x, v3.y, v3.z);

		FindNormal();
	}

	// Public functions
	MtTriangle& operator *= ( const MtMatrix4& matrix );

	void					FindPolygonCenter();
	void					FindPolygonRadius();
	void					FindNormal();
	BtFloat					SqrDistance( const MtVector3 &v3Point );
	
	BtBool					CollideWithPlane ( const MtPlane &plane, MtVector3 &intersection);
	BtBool					SphericalTest    ( const MtTriangle &other );
	BtU32					Collide          ( MtTriangle &tri2, MtVector3 intersection[6] );

	// Member variables
	MtVector3				a, b, c;
	BtFloat					radius;	
	MtVector3				center;
	MtVector3				normal;
};
