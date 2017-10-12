////////////////////////////////////////////////////////////////////////////////
// CgCard2.h

// Include guard
#pragma once
#include "RsMaterial.h"
#include "HlView.h"
#include "HlTimer.h"

class RsMaterial;

// Class definition
class CgCard2 : public HlView
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
	HlTimer							m_timer;
};
