//----------------------------------------------------------------------------
// BtCRC.h

#pragma once
#include "BtTypes.h"

class BtCRC
{
public:

	static BtU32 GenerateHashCode( const BtU8* pData, BtU32 nLength );
};
