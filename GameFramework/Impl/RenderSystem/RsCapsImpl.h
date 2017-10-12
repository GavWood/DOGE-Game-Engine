////////////////////////////////////////////////////////////////////////////////
// RsCapsWinGL.h

#pragma  once
#include "BtArray.h"
#include "RsCaps.h"

// Consts

// Class Declaration
class RsCapsWinGL : public RsCaps
{
public:

	RsCapsWinGL();

	BtBool												IsMultipleSimultaneousRenderTargets() const;
	BtU32												GetNumModes();
	RsCapsMode											GetMode( BtU32 modeIndex );
	void												GetModes();

private:

	BtArray< RsCapsMode, 256 >							m_modes;
};
