////////////////////////////////////////////////////////////////////////////////
// BtCRC.cpp

#include "BtCRC.h"

//----------------------------------------------------------------------------
// GenerateHashCode

BtU32 BtCRC::GenerateHashCode( const BtU8* pData, BtU32 nLength )
{
	BtU32 nHashCode = 0;
	BtU32 t;

	while( nLength )
	{
		t = *( pData + nLength - 1 );
		nHashCode = t + ( nHashCode << 6 ) + ( nHashCode << 16 ) - nHashCode;
		--nLength;
	}
	return nHashCode;
}
