////////////////////////////////////////////////////////////////////////////////
// BtTimeDX11.h

// Include guard
#pragma once
#include "BtBase.h"
#include <windows.h>
#include <time.h>

// Class declaration
class BtTimeDX11
{

	public:
	
		static void					Init();
		static BtFloat				GetElapsedTimeInSeconds();
		
		static BtU32				Day();
		static BtU32				Month();
		static BtU32				Year();

	private:

		static BtFloat				m_start;
};
