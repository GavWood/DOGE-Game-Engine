// Local includes /////////////////////////////////////////////////////////////
#include "MtVector3.h"
#include "MtMatrix4.h"
#include "MtQuad.h"

// Public functions ///////////////////////////////////////////////////////////

CQuad::CQuad( MtVector3 &a, MtVector3 &b, MtVector3 &c, MtVector3 &d )
{
	m_v1 = a;
	m_v2 = b;
	m_v3 = c;
	m_v4 = d;
}

CQuad& CQuad::operator *= ( const MtMatrix3& transform )
{
	m_v1 *= transform;
	m_v2 *= transform;
	m_v3 *= transform;
	m_v4 *= transform;
	return *this;
}
