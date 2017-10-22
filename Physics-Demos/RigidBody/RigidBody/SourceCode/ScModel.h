////////////////////////////////////////////////////////////////////////////////
// SbModel.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "MtSphere.h"
#include "DyBody.h"

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
	void							Reset();
	void							Update( RsCamera &camera );
	void							Render( RsCamera &camera );

	// Accessors

private:

	// Private functions
	
	// Private members
	SgNode						   *m_pLargeCube;
	SgNode						   *m_pSmallCube;

	DyBody							m_largeBody;
	DyBody							m_smallBody;

	BtFloat							m_time;
	RsShader					   *m_pShader;
    MtSphere                        m_sphere;	
};
