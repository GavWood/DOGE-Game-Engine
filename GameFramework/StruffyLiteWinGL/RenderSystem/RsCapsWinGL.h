////////////////////////////////////////////////////////////////////////////////
// RsCapsWinGL.h

#ifndef __RsCapsWinGL_h__
#define __RsCapsWinGL_h__

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

#endif // __RsCapsWinGL_h__
