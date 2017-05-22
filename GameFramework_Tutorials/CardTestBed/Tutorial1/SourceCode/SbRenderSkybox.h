////////////////////////////////////////////////////////////////////////////////
// SbRenderSkybox.h

// Include guard
#pragma once
#include "MtMatrix3.h"
#include "MtVector3.h"

class SgNode;
class BaArchive;
class RsMaterial;
class RsShader;
class MuModelFlyingField;
struct RsVertex3;
struct MuMVC;

// Class definition
class SbRenderSkybox
{
public:

	// Public functions
	void							Setup( BaArchive *pArchive );
	void							Update( MtVector3 v3Translation );
	void							Render();

	// Accessors

private:

	// Private functions
	void							Rotate( RsVertex3* pVertex );

	// Private members	
	// Resources
	SgNode						   *m_pFlyer;
	RsMaterial*						m_pBackMaterial;
	RsMaterial*						m_pFrontMaterial;
	RsMaterial*						m_pLeftMaterial;
	RsMaterial*						m_pRightMaterial;
	RsMaterial*						m_pUpMaterial;
	MtVector3						m_v3Translation;
	RsShader					   *m_pShader;
};
