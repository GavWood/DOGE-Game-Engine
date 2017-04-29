////////////////////////////////////////////////////////////////////////////////
// BtTime.h

// Include guard
#pragma once
#include "BtBase.h"

// Class declaration
class BtTime
{

	public:
	
		static BtU32				GetTime();
		static BtFloat				GetTick();
		static void					SetTick( BtFloat tick );
		static BtFloat				GetElapsedTimeInMilliseconds();
		static BtFloat				GetElapsedTimeInSeconds();
		static BtU32				GetDay();
		static BtU32				GetMonth();
		static BtU32				GetYear();

private:

		static BtFloat				m_tick;
};
