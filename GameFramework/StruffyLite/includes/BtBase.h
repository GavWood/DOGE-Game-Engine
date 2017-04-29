////////////////////////////////////////////////////////////////////////////////
// BtBase.h

// Include guard
#pragma once
#include <assert.h>
#include "BtTypes.h"

// BtAssert
#define BtAssert(x) { assert(x); }

// BtUnusedVariable
#define BtUnusedVariable(x) { (void)(x); }

////////////////////////////////////////////////////////////////////////////////
// BtFloatToBtU32

inline BtU32 BtFloatToBtU32( BtFloat f ) { return *((BtU32*)&f); }

////////////////////////////////////////////////////////////////////////////////
// BtNearestPowerOf2

inline BtU32 BtNearestPowerOf2( BtU32 nValue )
{
	BtU32 nPower = 1;
	
	for( BtU32 nShift=0; nShift<31; nShift++ )
	{
		if( nPower >= nValue )
		{
			return nPower;
		}
		
		nPower = nPower << 1;
	}
	
	return nPower;
}

////////////////////////////////////////////////////////////////////////////////
// Swap a and b

template<class T> inline void BtSwap( T& a, T& b )
{
	T tmp=a;
	a=b;
	b=tmp;
}

////////////////////////////////////////////////////////////////////////////////
// Clamp the input to the specified range

template<class T, class U> inline T BtClamp( const T& v, const U& l, const U& h )
{
	return (v<l) ? l : (v>h) ? h : v;
}

