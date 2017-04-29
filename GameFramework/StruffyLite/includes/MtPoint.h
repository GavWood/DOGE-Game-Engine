////////////////////////////////////////////////////////////////////////////////
// MtPoint.h

// Include guard 
#ifndef __MtPoint_H__
#define __MtPoint_H__

// Forward prototypes
class MtPlane;
struct MtVector3;

// Class declaratiion
struct MtPoint
{
    // Constructors
	MtPoint( const MtVector3 &pos ) { m_v3Point = pos; }

	// Public functions
	BtFloat				SideOfPlane( const MtPlane &plane );
	static BtFloat		SideOfPlane( const MtVector3 &point, const MtPlane &plane );

	// Member variables
	MtVector3			m_v3Point;
};

#endif // __MtPoint_H__