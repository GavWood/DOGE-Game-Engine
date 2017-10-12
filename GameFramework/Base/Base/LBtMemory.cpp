////////////////////////////////////////////////////////////////////////////////
// LBtMemory.cpp

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "BtTypes.h"
#include "BtMemory.h"

BtU8* BtMemory::Allocate( BtU32 nSizeInBytes )
{
	//printf( "Memory %d\n", nSizeInBytes );
	
	return (BtU8*) malloc( nSizeInBytes );
}

BtU8* BtMemory::Allocate( BtU32 nSizeInBytes, BtU32 nAlignment )
{
	(void)nAlignment;
	return (BtU8*) Allocate( nSizeInBytes );
}

void BtMemory::Free( void *pMemory )
{
	free( pMemory );
}

void BtMemory::Copy( void* pDest, const void *pSource, BtU32 nSize )
{
	memcpy( pDest, pSource, nSize );
}

void BtMemory::SetWords( BtU32* pDest, BtU32 value, BtU32 numWords )
{
	BtU32 *pWord = (BtU32*)pDest;
	for (BtU32 i = 0; i < numWords; i++)
	{
		*pWord++ = value;
	}
}

void BtMemory::Set( void* pDest, BtU8 value, BtU32 nSize )
{
	memset(pDest, value, nSize);
}

BtBool BtMemory::Compare( const void* pDest, const void *pSource, BtU32 nSize )
{
	if( memcmp( pDest, pSource, nSize ) == 0 )
	{
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}
