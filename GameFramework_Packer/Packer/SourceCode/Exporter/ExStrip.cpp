////////////////////////////////////////////////////////////////////////////////
// ExStrip.cpp

// Includes
#include "ExStrip.h"
#include "ExSgNode.h"
//#include "NvTriStrip.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExGeometryOptimise::ExGeometryOptimise( ExMaterialBlock* pMaterialBlock, ExRenderBlock* pRenderBlock )
{
	m_pMaterialBlock = pMaterialBlock;
	m_pRenderBlock   = pRenderBlock;
}

////////////////////////////////////////////////////////////////////////////////
// CopyVertexNoMerge

void ExGeometryOptimise::CopyVertexNoMerge()
{
	BtU32 nIndices = (BtU32) m_pRenderBlock->m_indices.size();

	for( BtU32 iIndex=0; iIndex<nIndices; iIndex++ )
	{
		// Cache the original index
		BtU32 iOriginalIndex = m_pRenderBlock->m_indices[iIndex];

		// Cache each vertex
		ExVertex* pVertex = m_pRenderBlock->m_vertex[iOriginalIndex];

		// Calculate the hash code
		pVertex->CalculateHashCode();

		BtU32 nOptimisedVertexCount = (BtU32) m_pRenderBlock->m_pOptimisedVertex.size();
		m_pRenderBlock->m_pOptimisedVertex.push_back( pVertex );
		m_pRenderBlock->m_optimisedIndex.push_back( nOptimisedVertexCount );
	}

	// Validate the vertex
	for( BtU32 iIndex=0; iIndex<nIndices; iIndex++ )
	{
		BtU32 iOriginalIndex = m_pRenderBlock->m_indices[iIndex];
		BtU32 iNewIndex = m_pRenderBlock->m_optimisedIndex[iIndex];

		if( m_pRenderBlock->m_pOptimisedVertex[iNewIndex]->CRC() != m_pRenderBlock->m_vertex[iOriginalIndex]->CRC() )
		{
			ErrorLog::Fatal_Printf( "Vertex not optimised correctly through CRC.\r\n" );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MergeVertex

void ExGeometryOptimise::MergeVertex()
{
    CopyVertexNoMerge();
}

////////////////////////////////////////////////////////////////////////////////
// Strip

void ExGeometryOptimise::Strip()
{
	//SetCacheSize( CACHESIZE_GEFORCE3 );
	//SetStitchStrips( BtTrue );

	// Generate the strips
	BtU32 numIndices = (BtU32) m_pRenderBlock->m_optimisedIndex.size();

	if( numIndices > 66535 )
	{
		ErrorLog::Fatal_Printf( "Too many index." );
	}
	else
	{
		ErrorLog::Printf("Index %d.", numIndices );		
	}


	// Copy the optimised index straight into the stripp index
	for( BtU32 i=0; i<numIndices; i++ )
	{
		int index = m_pRenderBlock->m_optimisedIndex[i];
		m_pRenderBlock->m_strippedIndex.push_back(index);
	}
	return;

	/*
	BtU16 numGroups;
	PrimitiveGroup *pPrimitiveGroups;
	BtBool stripped = GenerateStrips( (BtU16*) &m_pRenderBlock->m_optimisedIndex[0],
									  numIndices,
									  &pPrimitiveGroups,
								 	  &numGroups,
									  validate );
	if( stripped == BtFalse )
	{
		//ErrorLog::Fatal_Printf( "ExStrip::GenerateStrips failed to strip." );
	}
	else if( numGroups > 1 )
	{
		ErrorLog::Fatal_Printf( "ExStrip::GenerateStrips  returned more than one group. It didn't use degenerates." );
	}

	// Clear the index
	m_pRenderBlock->m_strippedIndex.clear();

	// Dump out the primitives
	PrimitiveGroup *pPrimitiveGroup = &pPrimitiveGroups[0];

	m_pRenderBlock->m_primitiveType = ExPT_STRIP;

	for( BtU32 i=0; i<pPrimitiveGroup->numIndices; i++ )
	{
		// Add the optimised index
		m_pRenderBlock->m_strippedIndex.push_back( pPrimitiveGroup->indices[i] );
	}

	free( pPrimitiveGroups );

	int a=0;
	a++;
	*/
}
