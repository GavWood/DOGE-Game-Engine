////////////////////////////////////////////////////////////////////////////////
// ExMaterialBlock.h

#pragma once
#include "BaArchive.h"
#include "BtTypes.h"
#include "ExMaterialList.h"
#include "ExVertexJointWeightPairs.h"
#include "ExVertex.h"
#include "RsPrimitive.h"
#include <vector>

class FCDMaterialBlock;
class FCDEntityInstance;
class ExMaterialBlock;

enum ExPrimitiveType
{
	ExPT_LIST,
	ExPT_STRIP,
	ExPT_FAN,
};

class ExRenderBlock
{
public:

	ExRenderBlock();

	void									Empty();
	BtS32									addBoneToPalette( BtS32 BoneIndex );
	BtBool									addTriangleToGroup( ExMaterialBlock* pMaterialBlock, BtU32 iIndex );

	BtS32									m_bonePalette[MaxBoneMatrices];
	BtS32									m_bonePaletteSize;

	std::vector<ExVertex*>					m_vertex;
	std::vector<BtU32>						m_indices;

	std::vector<ExVertex*>					m_pOptimisedVertex;
	std::vector<BtU32>						m_optimisedIndex;
	std::vector<BtU32>						m_strippedIndex;
	
	BaRenderBlockFileData					m_fileData;
	std::vector<RsIndexedPrimitive>			m_primitives;

	ExPrimitiveType							m_primitiveType;
};

inline ExRenderBlock::ExRenderBlock()
{
	m_primitiveType = ExPT_LIST;
	Empty();
}

inline void ExRenderBlock::Empty()
{
	for( BtU32 iBoneMatrix=0; iBoneMatrix<MaxBoneMatrices; iBoneMatrix++ )
	{
		m_bonePalette[iBoneMatrix] = -1;
	}
	m_bonePaletteSize = 0;
	m_indices.clear();
	m_vertex.clear();
	m_pOptimisedVertex.clear();
	m_optimisedIndex.clear();
}

class ExMaterialBlock
{
public:

	~ExMaterialBlock()
	{
		BtU32 nSize;

		nSize = (BtU32) m_pVertex.size();

		for( BtU32 i=0; i<nSize; i++ )
		{
			delete m_pVertex[i];
		};
	}

	BaMaterialBlockFileData					m_fileData;
	ExMaterial*								m_pMaterial;
	std::vector<ExRenderBlock>				m_renderBlocks;
	std::vector<ExVertex*>					m_pVertex;
	std::vector<BtU32>						m_indices;
};
