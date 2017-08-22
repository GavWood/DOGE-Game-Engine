////////////////////////////////////////////////////////////////////////////////
// BCdxf.h

// Include guard
#ifndef __BCdxf_h__
#define __BCdxf_h__

#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "McTextFileReader.h"
#include "McVanilla.h"
#include "UtFilename.h"

// Class definition
class BCdxf
{

public:

	// Public functions
	void						Load( UtFilename* pFilename );

	// Accessors
	MCVanilla*					GetVanilla();

private:

	MCVanilla					m_vanilla;
};

////////////////////////////////////////////////////////////////////////////////
// GetVanilla

inline MCVanilla* BCdxf::GetVanilla()
{
	return &m_vanilla;
}

#endif // __BCdxf_h__
