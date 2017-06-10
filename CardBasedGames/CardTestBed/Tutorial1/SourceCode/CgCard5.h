////////////////////////////////////////////////////////////////////////////////
// CgCard5.h

// Include guard
#pragma once
#include "HlView.h"
#include "SdSound.h"

// Class definition
class CgCard5 : public HlView
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
	SdSound					 	   *m_pSound;
};
