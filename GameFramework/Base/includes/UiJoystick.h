////////////////////////////////////////////////////////////////////////////////
/// UiJoystick.h

#ifndef __UiJoystick_h__
#define __UiJoystick_h__

#include "BtTypes.h"

// Class Declaration
class UiJoystick
{
public:

	// Instance
	static UiJoystick  			   *pInstance();

	// Accessors
protected:

	static UiJoystick			   *m_pInstance;
};

#endif // __UiJoystick_h__
