////////////////////////////////////////////////////////////////////////////////
// SbSprite.h

// Include guard
#pragma once
#include "BtTypes.h"

class RsSprite;
class BaArchive;

// Class definition
class SbSprite
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update();
	void							Render();

	// Accessors

private:

	// Private functions

	// Private members
	RsSprite					   *m_pSprite;
};
