////////////////////////////////////////////////////////////////////////////////
/// UiMouse.h

#ifndef __UiMouse_h__
#define __UiMouse_h__

#include "BtTypes.h"

// Class Declaration
class UiMouse
{
public:

	// Instance
	static UiMouse  			   *pInstance();

	// Accessors
	virtual MtVector2 				GetPosition() = 0;

protected:

	static UiMouse				   *m_pInstance;
};

#endif // __UiMouse_h__

