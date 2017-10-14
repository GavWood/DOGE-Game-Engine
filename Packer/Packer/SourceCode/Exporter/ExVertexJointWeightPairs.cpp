////////////////////////////////////////////////////////////////////////////////
// ExVertexJointWeightPairs.cpp

#include "ExVertexJointWeightPairs.h"

////////////////////////////////////////////////////////////////////////////////
// Normalise

void ExVertexJointWeightPairs::Normalise()
{
	BtFloat fTotal = 0;

	for( BtU32 iWeight=0; iWeight<m_nWeightsUsed; iWeight++ )
	{
		// Cache each pair
		const ExJointWeightPair& pair = m_jointWeightPair[iWeight];

		// Sum the weights
		fTotal += pair.m_fWeight;
	}

	for( BtU32 iWeight=0; iWeight<m_nWeightsUsed; iWeight++ )
	{
		// Cache each pair
		ExJointWeightPair& pair = m_jointWeightPair[iWeight];

		pair.m_fNormalisedWeight = pair.m_fWeight / fTotal;
	}
};
