////////////////////////////////////////////////////////////////////////////////
// BCMetasequoiaExport.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "McTextFileReader.h"
#include "McVanilla.h"

// Class definition
class BCMetasequoiaExport
{
public:

	// Public functions
	void						Export( const MCVanilla* pVanilla, const BtChar* pFilename );
	
private:

};
