////////////////////////////////////////////////////////////////////////////////
// SbFrustum.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "MtMatrix4.h"
#include "BtArray.h"
#include "RsCamera.h"
#include "MtVector3.h"
#include "RsMaterial.h"

// Class definition
class SbFrustum
{
public:

	// Public function
	void				Setup( BaArchive *pArchive );
	void				Update( const MtMatrix3 &m3Rotation, MtVector3 v3Position, BtFloat completeFieldOfView, BtFloat aspect );
	void				Render();

	// Accessors
	MtVector3		    *GetVertex();

private:
	
	// Private functions

	MtVector3			m_rect[5];
	RsMaterial		   *m_pWhite3;
};

