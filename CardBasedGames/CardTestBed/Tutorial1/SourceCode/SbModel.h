////////////////////////////////////////////////////////////////////////////////
// SbModel.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"

class RsModel;
class BaArchive;
class SgAnimator;

// Class definition
class SbModel
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive, BaArchive *pAnimArchive  );
	void							Update();
	void							Render();

	// Accessors

private:

	// Private functions
	
	// Private members
	SgNode						   *m_pSphere;
    SgNode                         *m_pSkybox;
	RsFont						   *m_pFont;
	RsShader					   *m_pShader;

	SgNode						   *m_pOwl;
	SgAnimator					   *m_pAnimator;
	BtFloat							m_frameIndex;
};
