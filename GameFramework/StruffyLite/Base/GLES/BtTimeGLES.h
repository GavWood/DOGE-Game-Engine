////////////////////////////////////////////////////////////////////////////////
// BtTimeWin32.h

// Include guard
#ifndef __BtTimeWin32_H
#define __BtTimeWin32_H

#include "BtBase.h"
#include <time.h>

// Class declaration
class BtTimeGLES
{

	public:
	
		static void					Init();
		static BtFloat				GetElapsedTimeInSeconds();
		
		static BtU32				Day();
		static BtU32				Month();
		static BtU32				Year();
};

#endif // __BtTimeWin32_H
