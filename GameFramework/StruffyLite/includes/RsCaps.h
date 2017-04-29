////////////////////////////////////////////////////////////////////////////////
/// RsCaps.h

#ifndef __RsCaps_h__
#define __RsCaps_h__

#include "BtTypes.h"

struct RsCapsMode
{
	BtU32							m_index;
	BtU32							m_width;
	BtU32							m_height;
	BtU32							m_refreshMode;
};

// Class Declaration
class RsCaps
{
public:

	// Accessors
	virtual BtBool					IsMultipleSimultaneousRenderTargets() const = 0;
	virtual BtU32					GetNumModes() = 0;
	virtual RsCapsMode				GetMode( BtU32 modeIndex ) = 0;
};

#endif // __RsCaps_h__
