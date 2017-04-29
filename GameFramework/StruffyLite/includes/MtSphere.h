////////////////////////////////////////////////////////////////////////////////
// MtSphere.h

// Include guard
#pragma once

// Forward declaration
#include "MtVector3.h"
#include "MtMatrix4.h"

class MtSphere;

// Class definition
class MtSphere
{
public:

	// Constructors
	MtSphere();
	MtSphere( const MtVector3& v3Position, BtFloat radius );

	// Public functions
	const MtVector3&				GetPosition() const;
	BtFloat							GetRadius() const;
	void							Transform( const MtMatrix4& m4Transform );
	void							ExpandBy( const MtVector3& v3Position );
	void							ExpandBy( const MtSphere& other );
	BtBool			 				SphericalTest( const MtSphere& other );	
	BtBool							Intersect( MtVector3 rayStart,
											   MtVector3 rayEnd,
											   MtVector3& v3Intersection1,
											   MtVector3& v3Intersection2
											 ) const;

private:

	// Member variables
	MtVector3		 				m_v3Position;
	BtFloat			 				m_radius;
};

////////////////////////////////////////////////////////////////////////////////
// Position

inline const MtVector3& MtSphere::GetPosition() const
{
	return m_v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// Radius

inline BtFloat MtSphere::GetRadius() const
{
	return m_radius;
}
