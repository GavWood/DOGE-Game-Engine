////////////////////////////////////////////////////////////////////////////////
// SbTexture.h

// Include guard
#pragma once
#include "BtTypes.h"

class RsTexture;
class BaArchive;

// Class definition
class SbTexture
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
	RsTexture					   *m_pTexture;
};
