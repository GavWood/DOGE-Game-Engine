////////////////////////////////////////////////////////////////////////////////
// SgBlendShape.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"

// Class Declaration
class SgBlendShape
{
public:
	
	// Accessors
	virtual BtU32				Items() = 0;
	virtual void				Weight( BtU32 iIndex, BtFloat weight ) = 0;
	virtual BtFloat				Weight( BtU32 iIndex ) = 0;
	virtual BtFloat*			pWeights() const = 0;
};
