////////////////////////////////////////////////////////////////////////////////
// ExJointWeightPair.h

#ifndef __ExJointWeightPair_h__
#define __ExJointWeightPair_h__

#include "BtTypes.h"

class ExJointWeightPair
{
public:

	ExJointWeightPair()
	{
		m_fWeight = 0;
		m_fNormalisedWeight = 0;
		m_iBone = -1;
	}

	BtFloat m_fWeight;
	BtFloat m_fNormalisedWeight;
	BtS32 m_iBone;
};

#endif // __ExJointWeightPair_h__
