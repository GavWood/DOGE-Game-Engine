////////////////////////////////////////////////////////////////////////////////
// LBtMemory.h

#pragma once
#include "BtTypes.h"

// Class Declaration
class BtMemory
{
public:

	static BtU8*  Allocate( BtU32 nSizeInByes );
	static BtU8*  Allocate( BtU32 nSizeInByes, BtU32 nAlignment );
	static void   Free( void* pMemory );
	static void   FreeAligned( void* pMemory );
	static void   Copy( void* pDest, const void *pSource, BtU32 nSize );
	static void   SetWords( BtU32* pDest, BtU32 nValue, BtU32 numWords);
	static void   Set(void* pDest, BtU8 nValue, BtU32 nSize);
	static BtBool Compare( const void* pDest, const void *pSource, BtU32 nSize );
};
