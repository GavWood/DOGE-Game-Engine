////////////////////////////////////////////////////////////////////////////////
// ExVertexJointWeightPairs.h

#ifndef __ExVertexJointWeightPairs_h__
#define __ExVertexJointWeightPairs_h__

#include "BtTypes.h"
#include "ExJointWeightPair.h"
#include "RsVertex.h"

const BtU32 MaxBoneInfluence = 4;

class ExVertexJointWeightPairs
{
public:

	void					Normalise();

	ExJointWeightPair		m_jointWeightPair[MaxBoneInfluence];
	BtU32					m_nWeightsUsed;
};

#endif // __ExVertexJointWeightPairs_h__
