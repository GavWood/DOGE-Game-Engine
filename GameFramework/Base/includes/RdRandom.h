////////////////////////////////////////////////////////////////////////////////
/// RdRandom.h

#pragma once
#include "BtTypes.h"

// Class Declaration
class RdRandom
{
public:

	static void			SetRandomSeed();
	static void			SetSeed();
	static void			SetSeed( BtS32 Min );
	static BtS32		GetNumber( BtS32 lower, BtS32 upper );
    static BtFloat      GetFloat( BtFloat lower, BtFloat upper );
};

