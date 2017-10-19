////////////////////////////////////////////////////////////////////////////////
// CgCard7.h

// Include guard
#pragma once
#include "HlView.h"
#include "SdSound.h"
#include "BtQueue.h"

class SgNode;

// Class definition
class CgCard7 : public HlView
{
public:

	// Public functions
	void							Setup();
	void							Update();
	void							Render();

	void							OnEntry();
	void							OnExit();

	// Accessors

private:

	// Private functions

	// Private members
	SgNode						   *m_pNode;
    BtQueue<BtFloat, 64>           m_pitch;
    BtQueue<BtFloat, 64>           m_amplitude;
};
