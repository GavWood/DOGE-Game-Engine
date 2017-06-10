////////////////////////////////////////////////////////////////////////////////
// CgCard6.h

// Include guard
#pragma once
#include "HlView.h"
#include "SdSound.h"

class SgNode;

// Class definition
class CgCard6 : public HlView
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
};
