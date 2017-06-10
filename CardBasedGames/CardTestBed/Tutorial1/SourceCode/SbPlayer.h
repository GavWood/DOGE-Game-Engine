////////////////////////////////////////////////////////////////////////////////
// SbPlayer.h

// Include guard
#pragma once
#include "BtTypes.h"

class BaArchive;

// Class definition
class SbPlayer
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Reset();
	void							Update();
	void							Render();

	// Accessors

private:

	// Private functions
	
	// Private members
};
