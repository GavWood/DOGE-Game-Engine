////////////////////////////////////////////////////////////////////////////////
// CgCard3.h

// Include guard
#pragma once
#include "RsMaterial.h"
#include "HlView.h"

class RsMaterial;

// Class definition
class CgCard3 : public HlView
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
};
