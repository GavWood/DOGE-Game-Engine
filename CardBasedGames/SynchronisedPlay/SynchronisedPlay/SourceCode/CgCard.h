////////////////////////////////////////////////////////////////////////////////
// CgCard4.h

// Include guard
#pragma once
#include "RsMaterial.h"
#include "HlView.h"

class RsMaterial;
class BaArchive;
class RsSprite;

// Class definition
class CgCard : public HlView
{
public:

	// Public functions
	void							Setup(BaArchive *archive);
	void							Update();
	void							Render();

	void							OnEntry();
	void							OnExit();
	void							LoadCard( BtU32 index );

	// Accessors

private:

	// Private functions

	// Private members
	BtU32							m_pingCount = 0;
	MtVector2						m_v2Position;
	BaArchive					   *m_pArchive;
	RsSprite					   *m_pSprite;
};
