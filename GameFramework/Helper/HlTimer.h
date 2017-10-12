////////////////////////////////////////////////////////////////////////////////
// HlTimer

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"


// Class definition
class HlTimer
{
public:

	// Public functions
	HlTimer();

	void							ResetTimer( BtFloat duration = 0 );
	BtFloat							GetElapsedTime();
	BtFloat						    GetStartTime();
	BtBool							IsReady();

private:

	BtFloat							m_start;
	BtFloat							m_duration;
};
