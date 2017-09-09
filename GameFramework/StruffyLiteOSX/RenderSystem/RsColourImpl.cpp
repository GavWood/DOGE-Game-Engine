////////////////////////////////////////////////////////////////////////////////
// RsColourWinGL

// Includes
#include "RsColour.h"

BtU32 RsColour::asWord() const
{
	//WinGL = PF_R8G8B8A8
	return asABGR();
}