////////////////////////////////////////////////////////////////////////////////
// SbModel.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "MtSphere.h"

class RsModel;
class BaArchive;
class SgAnimator;

// Class definition
class ScModel
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update( RsCamera &camera );
	void							Render( RsCamera &camera );

	// Accessors

private:

	// Private functions
	
	// Private members
	SgNode						   *m_pCube;
	RsShader					   *m_pShader;
    MtSphere                        m_sphere;
};
