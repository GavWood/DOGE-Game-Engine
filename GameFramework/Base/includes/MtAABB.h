#////////////////////////////////////////////////////////////////////////////////
// MtAABB.h

// Include guard
#pragma once
#include "MtMath.h"
#include "MtVector3.h"

class MtAABB
{
public:

	// Constructor
	MtAABB();
	MtAABB( const MtVector3& Min, const MtVector3& Max );
	MtAABB( const MtVector3& v3Position );
	MtAABB( BtFloat x1, BtFloat y1, BtFloat z1, BtFloat x2, BtFloat y2, BtFloat z2 );

	// Functions
    BtBool					Intersect( const MtAABB& AABB ) const;
    BtBool					Contains( const MtAABB& AABB ) const;
    BtBool					TestPoint( const MtVector3& point ) const;
	MtAABB					Transform( const MtMatrix4& Transform );
	void					ExpandBy( const MtAABB& AABB );
	void					ExpandBy( const MtVector3& v3Position );
	
	// Accessors
	MtVector3				Dimension() const;
	BtFloat					Diameter() const;
	BtFloat					Radius() const;
	MtVector3				Center() const;

	void					Min( const MtVector3& Min );
	void					Max( const MtVector3& Max );
    MtVector3&				Min();
    MtVector3&				Max();
    const MtVector3&		Min() const;
    const MtVector3&		Max() const;

private:
	
	MtVector3				m_min;
	MtVector3				m_max;
};

////////////////////////////////////////////////////////////////////////////////
// Min

inline void MtAABB::Min( const MtVector3& Min )
{
	m_min = Min;
}

////////////////////////////////////////////////////////////////////////////////
// Max

inline void MtAABB::Max( const MtVector3& Max )
{
	m_max = Max;
}

////////////////////////////////////////////////////////////////////////////////
// Min

inline MtVector3& MtAABB::Min()
{
	return m_min;
}

////////////////////////////////////////////////////////////////////////////////
// Max

inline MtVector3& MtAABB::Max()
{
	return m_max;
}

////////////////////////////////////////////////////////////////////////////////
// Min

inline const MtVector3&	MtAABB::Min() const
{
	return m_min;
}

////////////////////////////////////////////////////////////////////////////////
// Max

inline const MtVector3& MtAABB::Max() const
{
	return m_max;
}
