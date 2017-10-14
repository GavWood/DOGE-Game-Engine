////////////////////////////////////////////////////////////////////////////////
// MCMetasequoia.h

// Include guard
#ifndef __MCMetasequoia_h__
#define __MCMetasequoia_h__

#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "McTextFileReader.h"
#include "McVanilla.h"

// Class definition
class MCMetasequoia
{
public:

	// Public functions
	void						Load( const BtChar* metasequoia );

	// Accessors
	MCVanilla*					GetVanilla();
	
private:

	BtS32						GetToken( const BtChar* tokenName, BtU32 iLine );
	void						ReadMaterial( BtU32 iLine );
	void						ReadObject( BtS32 iLine );
	void						ReadMetasequoiaFile();

	MCVanilla					m_vanilla;
	MCTextFileReader			m_reader;
	BtS32						m_currentDepth;
};

////////////////////////////////////////////////////////////////////////////////
// GetVanilla

inline MCVanilla* MCMetasequoia::GetVanilla()
{
	return &m_vanilla;
}

#endif // __MCMetasequoia_h__
