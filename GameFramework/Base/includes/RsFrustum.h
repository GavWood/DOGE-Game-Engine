////////////////////////////////////////////////////////////////////////////////
/// RsFrustum.cpp

#pragma once
#include "MtPlane.h"
#include "MtMatrix4.h"
#include "MtVector3.h"

class RsFrustum
{
public:
	
	RsFrustum();

	void			Set( const MtMatrix4& m4Transform );
	BtBool			IsInside( const MtVector3& v3Position ) const;
	BtBool			IsInside( const MtVector3& v3Position, BtFloat radius ) const;

private:

	MtVector3		GetNearestPointOnFrustum( const MtVector3& v3Position ) const;
	
	MtPlane			m_frustumPlanes[6];
	MtVector3		m_frustumPointMax;
	MtVector3		m_frustumPointMin;
};
