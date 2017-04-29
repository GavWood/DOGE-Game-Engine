////////////////////////////////////////////////////////////////////////////////
// HlDraw.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "RsRenderTarget.h"
#include "MtAABB.h"
#include "MtMatrix4.h"

class RsMaterial;
class BaArchive;
class SgNode;

// Class definition
class HlJoysticks
{
public:

	// Public functions
	static void							Setup(SgNode *pModel1, SgNode *pModel2);
		
	static void							Setup( RsMaterial *pMaterial2,
											   RsMaterial *pMaterial3,
											   RsMaterial *pMaterial3NoZTest = BtNull );

	static void							Render();

	// Accessors
	static void							SetZTest(BtBool isEnabled);

private:

	// Private functions

	// Private members
	static RsMaterial					*m_pMaterial2;
	static RsMaterial					*m_pMaterial3;
	static RsMaterial					*m_pMaterial3NoZTest;
	static BtBool						 m_ztest;
	static SgNode						*m_pModel[2];
};
