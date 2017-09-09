////////////////////////////////////////////////////////////////////////////////
// SbMaterial.h

// Include guard
#pragma once
#include "BtTypes.h"

class RsMaterial;
class BaArchive;
class RsTexture;
class SgNode;
class RsShader;

// Class definition
class SbMaterial
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
	RsMaterial					   *m_pWolf2;
};
