////////////////////////////////////////////////////////////////////////////////
// SbSound.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtSphere.h"
#include "MtVector3.h"
#include "RsCamera.h"

class BaArchive;

// Class definition
class SbSound
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
};
