////////////////////////////////////////////////////////////////////////////////
// HlUserData.h

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "TinyXML2.h"

class BaArchive;

// Class definition
class HlUserData
{
public:
	
	// Public functions
	static BtU32 GetSize( BaArchive *pArchive, const BtChar *name );
	static BtU8 *GetData( BaArchive *pArchive, const BtChar *name );
	static BtBool GetXML( tinyxml2::XMLDocument &document, BaArchive *pArchive, const BtChar *title);
	static BtBool GetXMLFromFile( tinyxml2::XMLDocument &document, BaArchive *pArchive, const BtChar *title);
};
